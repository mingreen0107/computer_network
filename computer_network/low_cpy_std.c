#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#define BUF_SIZE 100

int main(int argc, char* argv[]) {
	int src, dst;  // file descriptor for source and destination
	int read_cnt;  // the number of bytes 
	char buf[BUF_SIZE];

	/* file open for read-only: O_RDONLY */


	/* file open for writing: O_CREAT|O_WRONLY|O_TRUNC */


	if (src == -1 || dst == -1) {
		puts("file open error");
		return -1;
	}

	/* read data from the source and write the data to the destination*/

	close(src);
	close(dst);
	return 0;
}