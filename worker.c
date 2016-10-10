
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include<signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include<sys/wait.h>
#include <sys/un.h>
#include<errno.h>
#include<dirent.h>
#include<unistd.h>
#include<string.h>
#define NROW 1000
#define NCOL 1000
#define ROW_IN_PARTION 125
#define COL_IN_PARTION 125
#define SOCKNAME_BASE "./tmp/worker"
#define UNIX_PATH_MAX 200
#define M 32300
#define N_ROW_IN_PARTION = 125
#define N_COL_IN_PARTION = 125
int main(int argc, char* argv[])
{
  int fd_skt = 0;
  int fd_c = 0;
  char buffer[M];
  int worker_id = atoi(argv[1]);
  char *sock_name = malloc(sizeof(char)*100);
  sprintf(sock_name,"%s_%d",SOCKNAME_BASE,worker_id);
  struct sockaddr_un sa;
  strncpy(sa.sun_path,sock_name,UNIX_PATH_MAX);
  sa.sun_family = AF_UNIX;
  fd_skt=socket(AF_UNIX,SOCK_STREAM,0);
  bind(fd_skt,(struct sockaddr *)&sa,sizeof(sa));
  listen(fd_skt,SOMAXCONN);
  printf("Waiting connection");
  fd_c=accept(fd_skt,NULL,0);
  read(fd_c,buffer,M);
}
