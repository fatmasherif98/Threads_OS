#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

struct result1_struct{
int row; //struct for row by row
};

struct result2_struct{ //struct for element by element
int row;
int column;
};

int n1,m1,n2,m2;
int *result1;
int *result2;
int *matrix1;
int *matrix2;


void* computeElement( void* arg){ //fn to compute element
    struct result2_struct *casted_arg = ( struct result2_struct*) arg;
    int row = casted_arg->row;
    int column = casted_arg->column;
    int i, sum=0;
    for( i=0; i<m1; i++)
    {
        sum += ( (*(matrix1 + row*m1 + i))* (*(matrix2 + i*m2 + column))); //m1 is no of columns in matrix1
    }
     *(result2 + row * m2 + column)= sum; //set element by it's value in global matrix
    pthread_exit(0);
}
void* computeRow( void* arg)
{
    struct result1_struct *casted_arg = ( struct result1_struct* ) arg;
    int row = casted_arg->row;
    int i=0,k=0, sum = 0;
    for( k=0; k<m2; k++)
    {
    sum = 0;
    for( i=0; i<n2; i++){
    sum += ( (*(matrix1 + row*m1 + i)) * (*(matrix2 + i*m2 + k))); //compute row

    }

    *(result1 + row*m2 + k) = sum;
    }
  pthread_exit(0);
}

void writeOutput(double timer1, double timer2){ //write output of both matricies to file

FILE *fptr = fopen("output.txt", "w");
if( fptr == NULL)
{
    printf("Can't open file\n");
    return;
}
int i,j;

for( i=0; i< n1; i++)
{
    for(j=0; j<m2; j++)
    {
        fprintf(fptr, "%d ", *(result1 + i*m2 + j) );
    }
    fprintf(fptr,"\n");
}
fprintf(fptr, "%lf\n", timer1); //print time for row by row

for( i=0; i< n1; i++)
{
    for(j=0; j<m2; j++)
    {
        fprintf(fptr, "%d ", *(result2 + i*m2 + j));
    }
    fprintf(fptr,"\n");
}

fprintf(fptr, "%lf\n", timer2); //print time for element by element
fclose(fptr);
}
int main()
{
    FILE *fptr = fopen("input.txt", "r");
    fscanf(fptr, "%d %d", &n1, &m1); //scan input
    matrix1 = (int *)malloc(n1*m1 * sizeof(int));
    int i=0,j=0;
    for(i = 0; i<n1; i++)
    {
        for(j=0; j<m1; j++)
        {
            fscanf(fptr,"%d", (matrix1 + i*m1 + j));
        }
    }
    fscanf(fptr, "%d %d", &n2, &m2);
    matrix2 = (int *)malloc(n2*m2 * sizeof(int));
    for(i = 0; i<n2; i++)
    {
        for(j=0; j<m2; j++)
        {
            fscanf(fptr,"%d",(matrix2 + i*m2 + j));
        }
    }
    fclose(fptr);
     result1 = (int *)malloc(n1*m2* sizeof(int)); //allocate global matricies
     result2 = (int *)malloc(n1*m2*sizeof(int));
    clock_t timer1; //start clock
    timer1 = clock();
    struct result1_struct Tarray1[n1]; //create structs for row by row


    pthread_t threadIds1[n1]; //create threads for row by row

    for( i=0; i<n1; i++)
    {
         Tarray1[i].row = i;
        pthread_create(&threadIds1[i],NULL,computeRow , &Tarray1[i]);
    }

    for(i=0; i<n1; i++)
    {
    pthread_join(threadIds1[i],NULL); //wait for threads to finish
    }

    timer1 = clock() - timer1;
/////////////////////COMPUTE BY ELEMENT?////////////////////

clock_t timer2;
timer2 = clock();
struct result2_struct Tarray2[n1*m2];
pthread_t   threadIds2[n1*m2];
for(i=0; i<n1; i++)
{
    for(j=0; j<m2; j++)
    {
        int index = (i*m2) + j;
        Tarray2[index].row = i;
        Tarray2[index].column = j;
        pthread_create(&threadIds2[index],NULL,computeElement , &Tarray2[index]);
    }
}

for(i=0; i<(n1*m2); i++)
    {
    pthread_join(threadIds2[i],NULL);
    }

timer2 = clock() - timer2;

    writeOutput( ( (double)timer1)/CLOCKS_PER_SEC, ( (double)timer2)/CLOCKS_PER_SEC );
free(result1);
free(result2);
free(matrix1);
free(matrix2);


    return 0;
}
