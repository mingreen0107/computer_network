#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char* message);

//int argc���� ./hello �϶��� 1 ./hello 1198 �϶��� 2, ���ڿ��� ������ �ٷ� �� char* ���
int main(int argc, char* argv[])
{
	int serv_sock;
	int clnt_sock;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	//Ŭ���̾�Ʈ�� ������ �����ߴٴ� ��ȣ�� �޾��� �� ������ ������ �޽���
	char message[] = "Hello World!";

	// ./hello 1198 �̷��� �����ؾ��� ���ư��ٴ� ��
	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");

	//������ ����� ����ü �ʱ�ȭ
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	//sockaddr�� ���·� �ޱ� ���� ����ȯ
	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");

	//����� �� �� �ֱ� ������ ��� ���� ���Ͼ��̵� �ʿ�-serv_sock
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	clnt_addr_size = sizeof(clnt_addr);
	//�޾� �帮�� �Լ�
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	//-1�� �ƴϸ� ����� ����ȴٴ� ��
	if (clnt_sock == -1)
		error_handling("accept() error");

	//read write �ϸ鼭 ��
	write(clnt_sock, message, sizeof(message));
	close(clnt_sock);
	close(serv_sock);
	return 0;
}
//������ �ȴٸ� listen�Լ����� ���� ��

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}