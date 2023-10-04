#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char* message);

//int argc에는 ./hello 일때는 1 ./hello 1198 일때는 2, 문자열을 여러개 다룰 때 char* 사용
int main(int argc, char* argv[])
{
	int serv_sock;
	int clnt_sock;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	//클라이언트가 서버로 접속했다는 신호를 받았을 때 서버가 보내는 메시지
	char message[] = "Hello World!";

	// ./hello 1198 이렇게 실행해야지 돌아간다는 뜻
	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");

	//위에서 선언된 구조체 초기화
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	//sockaddr의 형태로 받기 위한 형변환
	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");

	//통신을 할 수 있기 때문에 듣기 시작 소켓아이디 필요-serv_sock
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	clnt_addr_size = sizeof(clnt_addr);
	//받아 드리는 함수
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	//-1이 아니면 제대로 진행된다는 뜻
	if (clnt_sock == -1)
		error_handling("accept() error");

	//read write 하면서 끝
	write(clnt_sock, message, sizeof(message));
	close(clnt_sock);
	close(serv_sock);
	return 0;
}
//실행이 된다면 listen함수에서 진행 중

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}