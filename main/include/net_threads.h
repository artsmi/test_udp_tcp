#ifndef NET_THREADS_H
#define NET_THREADS_H

extern void* simple_sample_start(void* pdata);
extern void* udp_client_pusher_start(void* pdata);
extern void* udp_server_start(void* pdata);
extern void* tcp_server_start(void* pdata);
extern void* tcp_client_start(void* pdata);

#endif  // NET_THREADS_H