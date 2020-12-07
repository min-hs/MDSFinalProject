#ifndef	RPI_REGS_H
#define RPI_REGS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void server_init();
void server_close();
void server_accept();
ssize_t server_receive();

void client_close();


#endif