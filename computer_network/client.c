#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1000

void error_handling(char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char* argv[]){

    int sock;
    struct sockaddr_in serv_addr;
    char message[BUF_SIZE]; // ũ�� ����
    int str_len;
    char username[100]; // �̸��� �ޱ� ����

    if (argc != 3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    // ����ڷκ��� �̸� �Է� �ޱ�
    printf("input your name: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error!");
    // ����� ������ ����� �̸� ����
    send(sock, username, strlen(username), 0);

    // �޽��� ���� �� ���� �ݺ�
    while (1) {
        printf("input message(exit �� ����): ");
        fgets(message, BUF_SIZE, stdin);
        message[strcspn(message, "\n")] = '\0';

        // �޽����� "exit"�� ��� ���α׷� ����
        // -> �������� ���� ����Ͽ� ���� ������ ���� ���Դ��� �̸��� printf�ϴ� �Լ��� ����
        // �������� �����ְ� �����ϴ�
        if (strcmp(message, "exit") == 0) {
            printf("program exit\n");
            close(sock);
            exit(0);
        }
        // ������ �޽��� ����
        send(sock, message, strlen(message), 0);
        // �޽��� ���� �ʱ�ȭ
        memset(message, 0, BUF_SIZE);

        str_len = read(sock, message, sizeof(message) - 1);

        if (str_len == -1)
            error_handling("read() error!");

        printf("server response: %s\n", message);
    }
    close(sock);
    return 0;
}