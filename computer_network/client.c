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
    char message[BUF_SIZE]; // 크기 설정
    int str_len;
    char username[100]; // 이름을 받기 위해

    if (argc != 3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    // 사용자로부터 이름 입력 받기
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
    // 연결된 서버에 사용자 이름 전송
    send(sock, username, strlen(username), 0);

    // 메시지 전송 및 수신 반복
    while (1) {
        printf("input message(exit 시 종료): ");
        fgets(message, BUF_SIZE, stdin);
        message[strcspn(message, "\n")] = '\0';

        // 메시지가 "exit"인 경우 프로그램 종료
        // -> 여러명일 때를 고려하여 누가 나가고 누가 들어왔는지 이름을 printf하는 함수로 수정
        // 서버에게 보내주고 종료하는
        if (strcmp(message, "exit") == 0) {
            printf("program exit\n");
            close(sock);
            exit(0);
        }
        // 서버에 메시지 전송
        send(sock, message, strlen(message), 0);
        // 메시지 버퍼 초기화
        memset(message, 0, BUF_SIZE);

        str_len = read(sock, message, sizeof(message) - 1);

        if (str_len == -1)
            error_handling("read() error!");

        printf("server response: %s\n", message);
    }
    close(sock);
    return 0;
}