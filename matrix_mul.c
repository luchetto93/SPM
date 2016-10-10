#include<stdio.h>
#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<time.h>

int **myMat1 = NULL;
int **myMat2 = NULL;
int **myMat3 = NULL;
int **myMat4 = NULL;
int i = 0;
void *computeMul(void *arg)
{
   
   int *a = (int*)arg;
   int firstRow = *a * 100;
   int firstColumn = *a * 100;
   printf("%d-%d\n",firstRow,firstColumn);
   int partialSum = 0;
   int i = 0;
   int j = 0;
   int k = 0;
   int countCycle = 0;
   for(i=firstRow;i<firstRow+100;i++)
   {
       
       for(j=0;j<1000;j++)
       {
       partialSum = 0;
        for (k = 0; k < 1000; k++) {
            partialSum  = partialSum  + (myMat1[i][k] * myMat2[k][j]);
            
         }
         //printf("%d\n",partialSum);
         myMat3[i][j] = partialSum;
       }
   }
   //printf("%d\n",countCycle);
}
void computeMulSequential(int nrow,int ncol)
{
    int i = 0;
    int j = 0;
    int partialSum = 0;
    int k = 0;
    int sum = 0;
   for (i = 0; i < nrow; i++) {
      for (j = 0; j < ncol; j++) {
         sum = 0;
         for (k = 0; k < ncol; k++) {
            sum = sum + (myMat1[i][k] * myMat2[k][j]);
            
         }
         myMat4[i][j] = sum;
      }
   }
}
int **createMatrix(int nrow,int ncol)
{
  int **mat= (int**)malloc(sizeof(int*)*nrow);
  int i = 0;
  for(i=0;i<ncol;i++)
  {
    mat[i] = (int*)malloc(sizeof(int)*ncol);
  }
  return mat;
}
void insertInMatrix(int **matrix,int nrow,int ncol)
{
            int i = 0;
            int j = 0;
            for(i=0;i<nrow;i++)
            {
              for(j=0;j<ncol;j++)
              {
                  matrix[i][j] = rand()%2;
              }
            }

}
void viewMatrix(int **matrix,int nrow,int ncol)
{
     int i = 0;
     int j = 0;
     for(i=0;i<nrow;i++)
     {
         for(j=0;j<ncol;j++)
         {
            printf("%d-",matrix[i][j]);
         }
         printf("\n");
     }
}
int compareMatrix(int **mat1,int **mat2,int nrow,int ncol)
{
    int i = 0;
    int j = 0;
    int equal = 1;
    for(i=0;i<nrow;i++)
    {
       for(j=0;j<ncol;j++)
       {
           if(mat1[i][j] != mat2[i][j])
           {
            printf("Different %d - %d\n",i,j);
            equal = 0;
            break;
            break;
           }
       }
    }
    return equal;
}
int main(int argc,char* argv[])
{
    int nrow = atoi(argv[1]);
    int ncol = atoi(argv[2]);
   
    int i = 0;
    int compare = 1;
    myMat1 = createMatrix(nrow,ncol);
    myMat2 = createMatrix(nrow,ncol);
    myMat3 = createMatrix(nrow,ncol);
    myMat4 = createMatrix(nrow,ncol);
    insertInMatrix(myMat1,nrow,ncol);
    insertInMatrix(myMat2,nrow,ncol);
    //viewMatrix(myMat1,nrow,ncol);
    printf("\n");
    //viewMatrix(myMat2,nrow,ncol);
    pthread_t * idArray = (pthread_t *) calloc(sizeof(pthread_t),10); // we create an array of thread each of them identified by 
   // clock_t begin = clock();
   
    for(i=0; i<10; i++) {
    int * arg = (int *) calloc(sizeof(int),1);
    *arg = i;                     
    if((pthread_create(&idArray[i], NULL, computeMul, arg))!=0) {
      printf("Error while creating thread %d\n", i); 
    } else {
      printf("Thread %d created with success\n", i); 
    }
  }
  for(i=0; i<10; i++) {
    int * res; 
    if((pthread_join(idArray[i],(void **) &res)) != 0) {
      printf("Error"); 
    }else{
    printf("Thread  terminated\n"); 
  }
  }

   //computeMulSequential(nrow,ncol);
   //clock_t end = clock();
   
   //compare = compareMatrix(myMat3,myMat4,nrow,ncol);
   printf("%d\n",compare);
   //double time_spent = (double)(end-begin);
   //printf("Result %d\n",compare);
   //printf("Result from paralle\n");
  //viewMatrix(myMat3,nrow,ncol);
  printf("Result from sequential\n");
  // viewMatrix(myMat4,nrow,ncol);
   
    
    
    
    
}
