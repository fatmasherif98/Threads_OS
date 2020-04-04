#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct holder{
    int left;
    int right; //struct to send to the threads the left and right parameters for merge sort
};

int array_length; //global array length
int *my_array; //global array


void Merge( int l, int mid, int r) //merge function
{
    int left_len = mid-l+1; //calculate left length
    int right_len = r-mid;//calculate right length
    int arrLeft[left_len]; //create temporary arrays
    int arrRight[right_len];
    int i,j=0;
    for(i=l; i<=mid; i++)
    {
        arrLeft[j++] = my_array[i];
    }
    j=0;
    for(i=mid+1; i<=r; i++)
    {
        arrRight[j++] = my_array[i]; //fill temp arrays
    }
     i=0;j=0;
     int arr_counter=l;
     while( i< left_len && j< right_len) //merge loop
     {
            if( arrLeft[i] <= arrRight[j])
            {
                my_array[arr_counter++] = arrLeft[i++];
            }else
            {
                my_array[arr_counter++] = arrRight[j++];
            }
     }

     while( i < left_len)
     {
        my_array[arr_counter++] = arrLeft[i++];
     }
     while( j< right_len)
     {
        my_array[arr_counter++] = arrRight[j++];
     }
}


void* mergeSort( void* arg)
{


struct holder *input = ( struct holder*) arg;

    int l = input->left;
    int r = input->right;

    if( l>= r)
        pthread_exit(0); //if length 1 or less exit thread
    int mid = (l+r)/2;

    pthread_t left_thread; //create thread ids
    pthread_t right_thread;
    struct holder holder_left;
    struct holder holder_right;
    holder_left.left = l;
    holder_left.right = mid;
    holder_right.left = mid+1;
    holder_right.right = r;

    pthread_create(&left_thread,NULL,mergeSort , &holder_left); //create threads for left and right subarrays
    pthread_create(&right_thread,NULL,mergeSort , &holder_right);

    pthread_join(left_thread,NULL);
    pthread_join(right_thread,NULL);

    Merge(l, mid, r);
    pthread_exit(0);
}



void read_file(){  //read input from file

    FILE *fptr = fopen("input.txt", "r");
    fscanf(fptr, "%d", &array_length);
    int i=0;
    my_array = malloc(sizeof(int) * array_length);
    for(int i=0; i<array_length; i++)
    {
        fscanf( fptr, "%d", (my_array + i));
    }

    fclose(fptr);

}
int main()
{

     read_file();

     struct holder merge_holder;
     merge_holder.left=0;
     merge_holder.right = array_length-1;
     pthread_t thread_id;
     pthread_create(&thread_id,NULL,mergeSort , &merge_holder); //create first thread
     pthread_join(thread_id,NULL);
     int i=0;
     for(i=0; i< array_length;i++)
        printf("%d ", my_array[i]); //print sorted output

     free(my_array);
    return 0;
}
