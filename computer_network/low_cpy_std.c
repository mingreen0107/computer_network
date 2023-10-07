#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#define BUF_SIZE 100

int main(int argc, char* argv[]) {
	int src, dst;  // file descriptor for source and destination
	int read_cnt;  // the number of bytes 
	char buf[BUF_SIZE];

	/* file open for read-only: O_RDONLY */
	src = open("원본파일.txt", O_RDONLY);

	/* file open for writing: O_CREAT|O_WRONLY|O_TRUNC */
	dst = open("대상파일.txt", O_CREAT | O_WRONLY | O_TRUNC);

	if (src == -1 || dst == -1) {
		puts("file open error");
		return -1;
	}

	/* read data from the source and write the data to the destination*/
	while ((read_cnt = read(src, buf, BUF_SIZE)) > 0) {
		if (write(dst, buf, read_cnt) != read_cnt) {
			puts("file write error");
			return -1;
		}
	}

	close(src);
	close(dst);
	return 0;
}