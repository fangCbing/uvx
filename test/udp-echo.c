#include "../uvx.h"
#include <string.h>
#include <unistd.h>

// ���б�������������̣�
// ��һ����./udpecho
// �ڶ���: ./udpecho 127.0.0.1
// Ȼ�����߻ụ����Ϣ��һ���յ���Ϣ�����̴𸴸��Է������������ѭ����ȥ
// ����: Liigo <com.liigo@gmail.com>

static void on_recv(uvx_udp_t* xudp, void* data, ssize_t datalen, const struct sockaddr* addr, unsigned int flag) {
    char ip[16]; int port; uvx_get_ip_port(addr, ip, sizeof(ip), &port);
    printf("recv: %s  size: %d  from %s:%d \n", data, datalen, ip, port);
    int x = atoi(data);
    char newdata[16]; sprintf(newdata, "%d", x + 1);
    uvx_udp_send_to_addr(xudp, addr, newdata, strlen(newdata) + 1);
    sleep(1);
}

void main(int argc, char** argv) {
    char* target_ip = NULL;
    uv_loop_t* loop = uv_default_loop();
    uvx_udp_t xudp;
    uvx_udp_config_t config = uvx_udp_default_config(&xudp);
    config.on_recv = on_recv;

    if(argc > 1) target_ip = argv[1];
    if(target_ip == NULL) {
        uvx_udp_start(&xudp, loop, "0.0.0.0", 8008, config); // �󶨹̶��˿�
    } else {
        uvx_udp_start(&xudp, loop, NULL, 0, config); // ������˿�
        uvx_udp_send_to_ip(&xudp, target_ip, 8008, "1", 2); // ������Ϣ������������ײʽӦ��
    }

    uv_run(loop, UV_RUN_DEFAULT);
}
