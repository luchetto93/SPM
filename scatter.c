
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
#define UNIX_PATH_MAX 200
#define NROW 1000
#define NCOL 1000
#define PARTION_MATRIX_A 32300
#define PARTION_MATRIX_B 32300
#define NPROCESS 8
#define M 20010
#define SOCKNAME_BASE "./tmp/worker"
double **init_matrix(int nrow,int ncol)
{
    double **new_mat= (double**)malloc(sizeof(int*)*nrow);
    int i = 0;
    for(i=0;i<nrow;i++)
    {
       new_mat[i] = (double*)malloc(sizeof(int)*ncol);
    }
}
void matrix_fill(double **mat,int nrow,int ncol)
{
     int i = 0;
     int j = 0;
     for(i=0;i<nrow;i++)
     {
        for(j=0;j<ncol;j++)
        {
           mat[i][j] = rand() % 2;
        }
     
     }
}
char *string_copy(char *from_string,char *to_string,int lenght)
{
   int i = 0;
   //printf("%s\n",from_string);
   
   for(i=0;i<lenght;i++)
   {
      // printf("%c",from_string[i]);
      printf("%c",from_string[i]);
       
   }
   printf("\n");
}
char **create_socket_array()
{
    char **socket_array = (char**)malloc(sizeof(char*)*NPROCESS);
    char name_socket[100];
    int i = 0;
    for(i=0;i<NPROCESS;i++)
    {
       socket_array[i]= malloc(sizeof(char)*100);
    }
    
    for (i=0;i<NPROCESS;i++)
    {
       sprintf(name_socket,"%s%d.sck",SOCKNAME_BASE,i);
       strcpy(socket_array[i],name_socket);
  
    }
    return socket_array;
}
int return_socket_id(char *sock_name)
{
	struct sockaddr_un sa;
	int fd_skt=0;
	char buf[M];
	strncpy(sa.sun_path,sock_name,UNIX_PATH_MAX);
        sa.sun_family=AF_UNIX;
        fd_skt=socket(AF_UNIX,SOCK_STREAM,0);
        return fd_skt;
}
int * return_socket_array(char **socket_name,int n)
{
   int i = 0;
   int *socket_id_array = (int*)malloc(sizeof(int)*n);
   for(i=0;i<n;i++)
   {
       socket_id_array[i] = return_socket_id(socket_name[i]);
   }
   return socket_id_array;
}
void create_send_partion(double **input_matrix_A,double **input_matrix_B,int nrow,int ncol,char **socket_name)
{
	
	int fd_skt = 0;        
	int i = 0;
	int j = 0;
	int k = 0;
	int v = 0;
	int row_in_chunk = NROW/NPROCESS;
	int col_in_chunk = NCOL/NPROCESS;
	
        int finished = 0;
        char partion_matrix_A[PARTION_MATRIX_A];
        char partion_matrix_B[PARTION_MATRIX_B];
        int worker_to_transmit = 0;
        int *socket_array_id = return_socket_array(socket_name,NPROCESS);
        while(!finished)
        {
           for(i=0;i<nrow/NPROCESS;i++)
           {
               for(j=0;j<ncol/NPROCESS;j++)
               {
                  sprintf(partion_matrix_A,"%d|%d-",i,j);
                  sprintf(partion_matrix_B,"%d|%d-",i,j);
                  for(k=i*row_in_chunk;k<(i*row_in_chunk)+row_in_chunk;k++) /*we have to check also the bound of the matrix*/
                  {
                      for(v=j*col_in_chunk;v<(j*col_in_chunk)+ncol;v++) /*we have to check also the bound of the matrix*/
                      {
                       if(v==((j*col_in_chunk)+ncol-1)){
                       sprintf(partion_matrix_A,"%lf-",input_matrix_A[k][v]);
                       sprintf(partion_matrix_B,"%lf-",input_matrix_B[k][v]);
                       }
                       else{
                       sprintf(partion_matrix_A,"%lf|",input_matrix_A[k][v]);
                       sprintf(partion_matrix_B,"%lf|",input_matrix_B[k][v]);
                       
                       }
                      } 
                  }
                  write(socket_array_id[worker_to_transmit],partion_matrix_A,PARTION_MATRIX_A);
                  write(socket_array_id[worker_to_transmit],partion_matrix_B,PARTION_MATRIX_A);
                  worker_to_transmit = (worker_to_transmit+1) % NPROCESS;
               
               }
           }
        
        }
       // write(fd_skt,buf,M);
       
}
void print_string_array(int n_string,char **string_array)
{
    int i = 0;
    for(i=0;i<n_string;i++)
    {
       printf("%s\n",string_array[i]);
    }
}
int main()
{
    double **input_matrix_B = NULL;
    double **input_matrix_A = NULL;
    int inputStream[1];
    char **socket_array = NULL;
    socket_array = create_socket_array();
    print_string_array(NPROCESS,socket_array);
    //print_string_array(NPROCESS,socket_array);
    //
    //input_matrix_A = init_matrix(NROW,NCOL);
    //input_matrix_B = init_matrix(NROW,NCOL);
    //create_send_partion(input_matrix_A,input_matrix_B,NROW,NCOL);
    
}
