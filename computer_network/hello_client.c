#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1000

void error_handling(char* message);

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	char message[BUF_SIZE]; // 크기 변경
	int str_len;
	// 이름을 받기 위해
	char username[100];

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
		// enter 기준으로 문자열 처리
		message[strcspn(message, "\n")] = '\0';

		// 메시지가 "exit"인 경우 프로그램 종료
		if (strcmp(message, "exit") == 0) {
			printf("program exit\n");
			close(sock);
			exit(0);
		}
		// 서버에 메시지 전송
		send(sock, message, strlen(message), 0);
		// 메시지 버퍼 초기화 (전에 수신한 데이터와 헷갈리지 않도록)
		memset(message, 0, BUF_SIZE);

		str_len = read(sock, message, sizeof(message) - 1);
		if (str_len == -1)
			error_handling("read() error!");

		printf("server response: %s\n", message);
	}
	close(sock);
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}