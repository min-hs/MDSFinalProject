#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../inc/socket_server.h"

#define MAX_BUF 2

static int fd_l298n;
static int fd_hc_sr04_1; // front
static int fd_hc_sr04_2; // back
//static int fd_JODO;
//static int fd_BUZZER;

static char msg[3];
static unsigned char buf_out[MAX_BUF] = {'N', '9'};

static void motor_ctrl(char* msg) {
	if (buf_out[0] == msg[0] && buf_out[1] == msg[1]) {
		return;
	}
	buf_out[0] = msg[0];
	buf_out[1] = msg[1];
	write(fd_l298n, buf_out, MAX_BUF);
}

static void* func_hc_sr04(void* arg) {
	/*
	DATA = sensor data
	if (DATA <= 10)
		motor_ctrl(STOP MOTOR)
		alarm(buzzer)
	delay(0.1sec)		
	*/
	char buff[10], stop[2] = {'X', '0'};
	int dist1, dist2;
	int fd = *((int*)arg);
	while (1) {
		read(fd, buff, 4);
		dist1 = atoi(buff) % 1000;
		dist2 = atoi(buff) / 1000;
		printf("FRONT : %d\t BACK : %d\n", dist1, dist2);
		if ( (dist1 <= 10 && msg[0] == 'W') || (dist2 <= 10 && msg[0] == 'S') ) {
			motor_ctrl(stop);
		}
		usleep(100000);
	}
}

static void* func_WHATISJODO() {
	/*
	DATA = sensor data
	if (DATA)
		led_ctrl(ON)
	delay(0.1sec)	
	*/
}

static void* func_GYRO() {
	
}

int main(int argc, char *argv[])
{
	pthread_t thd_hc_sr04_1;
	pthread_t thd_hc_sr04_2;
	int res;
	
	// DEVICE SETUP START
	fd_l298n = open("/dev/l298n", O_RDWR);
	if(fd_l298n < 0)
	{
		perror("open()");
		exit(1);
	}	
	fd_hc_sr04_1 = open("/dev/hc_sr04_1", O_RDWR);
	if(fd_hc_sr04_1 < 0)
	{
		perror("open()");
		exit(1);
	}	
	fd_hc_sr04_2 = open("/dev/hc_sr04_2", O_RDWR);
	if(fd_hc_sr04_1 < 0)
	{
		perror("open()");
		exit(1);
	}
	
	// DEVICE SETUP END
	
	if(argc != 2){
		fprintf(stderr, "Usage: %s <Port>\n", argv[0]);
		exit(1);
	}
	
	// SERVER SETUP START
	server_init(argv[1]);
	// SERVER SETUP END
	
	// TODO hc_sr04 thread create
	//
	res = pthread_create(&thd_hc_sr04_1, NULL, func_hc_sr04, (void*)(&fd_hc_sr04_1));
	if (res < 0) {
		perror("thd_hc_sr04 create error : ");
		exit(0);
	}
	//res = pthread_create(&thd_hc_sr04_2, NULL, func_hc_sr04, (void*)(&fd_hc_sr04_2));
	//if (res < 0) {
	//	perror("thd_hc_sr04 create error : ");
	//	exit(0);
	//}
	// TODO sensor data reading thread create
	// 
		
	// SERVER COMMUNICATION START
	while(1){		
		server_accept();
	
		while(1){
			if (server_receive(msg) > 0) {				
				printf("!%c%c!\n", msg[0], msg[1]);
				motor_ctrl(msg);
			}
			else {
				break;
			}
		}
		client_close();
	}
	// SERVER COMMUNICATION END
		
	// TODO sensor data reading thread destroy
	//

	close(fd_l298n);
	close(fd_hc_sr04_1);
	close(fd_hc_sr04_2);
	server_close();

	return 0;
}
