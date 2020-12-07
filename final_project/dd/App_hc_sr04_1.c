#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>


char buff[100];


int main()
{
	int val;
	int fd = open("/dev/hc_sr04", O_RDWR);
	read(fd, buff, 4);
	printf("distance: %s\n", buff);

	close(fd);
	return 0;
}
