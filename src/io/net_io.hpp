#pragma once
#include <cstdint>
#include <string>
#include <stdexcept>
#include <vector>


#if defined(_WIN32)
  #define NOMINMAX
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #pragma comment(lib, "ws2_32.lib")
  using socklen_t = int;
  struct WsaInit {
    WsaInit(){ WSADATA d{}; if (WSAStartup(MAKEWORD(2,2), &d)!=0) throw std::runtime_error("WSAStartup"); }
    ~WsaInit(){ WSACleanup(); }
  };
  inline void net_init(){ static WsaInit g; }
  inline void net_close(SOCKET s){ closesocket(s); }
  using socket_t = SOCKET;
#else
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  #include <fcntl.h>
  #include <netdb.h>
  inline void net_init(){}
  inline void net_close(int s){ ::close(s); }
  using socket_t = int;
  #define INVALID_SOCKET (-1)
  #define SOCKET_ERROR   (-1)
#endif

inline void throw_last_net(const char* what){
#if defined(_WIN32)
  int ec = WSAGetLastError();
#else
  int ec = errno;
#endif
  throw std::runtime_error(std::string(what) + " (errno=" + std::to_string(ec) + ")");
}

inline socket_t connect_tcp(const char* host, uint16_t port){
  net_init();
  addrinfo hints{};
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  char port_str[16];
  snprintf(port_str, sizeof(port_str), "%u", (unsigned)port);

  addrinfo* res = nullptr;
  if (getaddrinfo(host, port_str, &hints, &res) != 0) throw_last_net("getaddrinfo");

  socket_t s = INVALID_SOCKET;
  for (addrinfo* p = res; p; p = p->ai_next){
    s = (socket_t)socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (s == INVALID_SOCKET) continue;
    if (::connect(s, p->ai_addr, (socklen_t)p->ai_addrlen) != SOCKET_ERROR){
      freeaddrinfo(res);
      return s;
    }
    net_close(s); s = INVALID_SOCKET;
  }
  freeaddrinfo(res);
  throw_last_net("connect");
}

inline socket_t listen_tcp(uint16_t port, int backlog=1){
  net_init();
  socket_t s = (socket_t)socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
  if (s == INVALID_SOCKET) throw_last_net("socket(AF_INET6)");

  int yes = 1;
  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(yes));
#ifdef IPV6_V6ONLY
  int v6only = 0; // allow both v4/v6
  setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&v6only, sizeof(v6only));
#endif

  sockaddr_in6 addr{};
  addr.sin6_family = AF_INET6;
  addr.sin6_addr   = in6addr_any;
  addr.sin6_port   = htons(port);
  if (bind(s, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR){ net_close(s); throw_last_net("bind"); }
  if (listen(s, backlog) == SOCKET_ERROR){ net_close(s); throw_last_net("listen"); }
  return s;
}

inline socket_t accept_one(socket_t lst){
  sockaddr_storage ss{};
  socklen_t slen = (socklen_t)sizeof(ss);
  socket_t s = (socket_t)accept(lst, (sockaddr*)&ss, &slen);
  if (s == INVALID_SOCKET) throw_last_net("accept");
  return s;
}

inline void send_all(socket_t s, const void* buf, size_t len){
  const char* p = (const char*)buf;
  while (len){
#if defined(_WIN32)
    int n = send(s, p, (int)len, 0);
#else
    ssize_t n = ::send(s, p, len, 0);
#endif
    if (n <= 0) throw_last_net("send");
    p += n; len -= (size_t)n;
  }
}

inline void recv_exact(socket_t s, void* buf, size_t len){
  char* p = (char*)buf;
  while (len){
#if defined(_WIN32)
    int n = recv(s, p, (int)len, MSG_WAITALL);
#else
    ssize_t n = ::recv(s, p, len, MSG_WAITALL);
#endif
    if (n <= 0) throw_last_net("recv");
    p += n; len -= (size_t)n;
  }
}

// big-endian u32
inline uint32_t be32(uint32_t x){
  return ((x & 0x000000FFu) << 24) |
         ((x & 0x0000FF00u) << 8 ) |
         ((x & 0x00FF0000u) >> 8 ) |
         ((x & 0xFF000000u) >> 24);
}

inline void send_frame(socket_t s, const std::string& payload){
  uint32_t n = be32((uint32_t)payload.size());
  send_all(s, &n, 4);
  send_all(s, payload.data(), payload.size());
}

inline std::string recv_frame(socket_t s){
  uint32_t n_be = 0;
  recv_exact(s, &n_be, 4);
  uint32_t n = be32(n_be);
  std::string out;
  out.resize(n);
  if (n) recv_exact(s, out.data(), n);
  return out;
}
