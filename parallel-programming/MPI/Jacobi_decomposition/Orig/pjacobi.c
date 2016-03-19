#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <assert.h>

//Reference implementation of Jacobi for mp2 (sequential)
//Constants are being used instead of arguments
#define BC_HOT  1.0
#define BC_COLD 0.0
#define INITIAL_GRID 0.5
#define TOL 1.0e-8
#define ARGS 5
//#define DEBUG 0

//MPI specific constants
#define MASTER 0               /* taskid of master */
#define SENDING_UP   0          /* setting a message tag */
#define SENDING_DOWN 1          /* setting a message tag */
#define MASTER 0 

typedef struct process_boundaries {
    int i_first;   
    int i_last;   
} process_boundary;

struct timeval tv;
double get_clock() {
   struct timeval tv; int ok;
   ok = gettimeofday(&tv, (void *) 0);
   if (ok<0) { printf("gettimeofday error");  }
   return (tv.tv_sec * 1.0 + tv.tv_usec * 1.0E-6);
}


double **create_matrix(int n) {
    int i;
    double **a;

    a = (double**) malloc(sizeof(double*)*n);
    for (i=0;i<n;i++) {
        a[i] = (double*) malloc(sizeof(double)*n);
    }

    return a;
}

void init_matrix(double **a, int n) {

    int i, j;
    
    for(i=0; i<n; i++) {
        for(j=0; j<n; j++)
            a[i][j] = INITIAL_GRID;
    }
}

void swap_matrix(double ***a, double ***b) {

    double **temp;

    temp = *a;
    *a = *b;
    *b = temp;    
}

void print_grid(double **a, int nstart, int nend) {

    int i, j;

    for(i=nstart; i<nend; i++) {
        for(j=nstart; j<nend; j++) {
            printf("%6.4lf ",a[i][j]);
        }
        printf("\n");
    }
}

void free_matrix(double **a, int n) {
    int i;
    for (i=0;i<n;i++) {
        free(a[i]);
    }
    free(a);
}

int main(int argc, char* argv[]) {
    int i,j,n,r,c,iteration,bclength,max_iterations;
    double **a, **b, maxdiff, gmaxdiff;
    double tstart, tend, ttotal;

    int rank, numprocessors, i_start, i_end;
    int top_row_of_sender, top_row_rank_plus_one, bottom_row_rank_minus_one, bottom_row_of_sender; 

    if (argc != ARGS) {
        fprintf(stderr,"Wrong # of arguments.\nUsage: %s N I R C\n", argv[0]);
        return -1;
    }
    n = atoi(argv[1]);
    max_iterations = atoi(argv[2]);
    r = atoi(argv[3]);
    c = atoi(argv[4]);

    //MPI Initialization
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocessors);

    MPI_Status status;
    MPI_Status WaitAllStatus[4];
    MPI_Request reqs[4];

    //We assumed that N is evenly divisible by the number of processors P.
    if (0 != n % numprocessors) {
        printf("It is assumed that N is evenly divisible by the number of processors P. Quitting...\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
        exit(1);
    }

    //Partitioning the rows among the diffrent processes
    process_boundary *proc_boundary = (process_boundary *) malloc(numprocessors*sizeof(process_boundary));

    int row_partition = n/numprocessors;

    if (0 == row_partition) {
        printf("It is assumed that N is greater than number of processors P. Quitting...\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
        exit(1);
    }

    for(i=0; i<numprocessors; i++) {
        proc_boundary[i].i_first = row_partition*i + 1 ;
        proc_boundary[i].i_last  = proc_boundary[i].i_first + row_partition;
    }
    
    //add 2 to each dimension to use sentinal values
    a = create_matrix(n+2);
    b = create_matrix(n+2);

    init_matrix(a,n+2);

    bclength = (n+2)/2;
    
    //Initialize the hot boundary
    for(i=0;i<bclength;i++) {
        a[i][0] = BC_HOT;
    }

    // Initialize the cold boundary
    for(j=n+2-bclength;j<n+2;j++) {
        a[n+1][j] = BC_COLD;
    }

    // Copy a to b
    for(i=0; i<n+2; i++) {
        for(j=0; j<n+2; j++) {
            b[i][j] = a[i][j];
        }
    }

    // Output initial grid
    #ifdef DEBUG
  //  if(0 == rank)
  //      print_grid(a,0,n+2);

  //  sleep(1);
  //  printf ("\n");
  //  if(1 == rank)
  //      print_grid(a,0,n+2);
    #endif


    // Main simulation routine
    iteration=0;
    gmaxdiff=1.0;

    tstart = get_clock();
    MPI_Barrier(MPI_COMM_WORLD);

    while(gmaxdiff > TOL && iteration<max_iterations) {

        // Initialize boundary values depending on which process they are alloted to.

        //Top and Top right taken care by rank 0
        if(0 == rank) {
          // Top
          for(j=0; j<n+2; j++)
              a[0][j] = a[1][j];

          //Top Right
          a[0][n+1] = a[0][n];
        }  

        //Bottom, Bottom right and Bottom Left taken care by rank numprocessors - 1
        if((numprocessors - 1) == rank) { 
          // Bottom
          for(j=0; j<n+2-bclength; j++)
              a[n+1][j] = a[n][j];

          //Bottom Left
          a[n+1][0] = a[n+1][1];
          //Bottom Right
          a[n+1][n+1] = a[n+1][n];
        } 
        
        //Left taken care in portions by corresponding process  
        i_start = proc_boundary[rank].i_first;
        i_end   = proc_boundary[rank].i_last;
        if(i_end >= bclength) {
            if(i_start < bclength) {
                i_start = bclength;
            } 
            for(i = i_start ; i < i_end; i ++) {
              // Left
              a[i][0] = a[i][1];
            }
        }
 
        //Right taken care in portions by corresponding process  
        i_start = proc_boundary[rank].i_first;
        i_end   = proc_boundary[rank].i_last;
        for(i = i_start ; i < i_end; i ++) {
          // Right
          a[i][n+1] = a[i][n];
        }

        //For a process of rank 'rank', the rows to send are top row and the bottom row
        top_row_of_sender       =  proc_boundary[rank].i_first;
        bottom_row_of_sender    =  proc_boundary[rank].i_last  -  1;
        //For a process of rank 'rank', the rows to receive are bottom row of process rank - 1 and the top row
        //of process rank + 1 
        bottom_row_rank_minus_one =  proc_boundary[rank-1].i_last - 1 ;
        top_row_rank_plus_one     =  proc_boundary[rank+1].i_first;

        int is_only_one_proc = (0 == numprocessors - 1 );
        maxdiff = 0.0;
    
        if(0 == is_only_one_proc) {
            //For process 0,            send own bottom row 
            //For process 1,2,...,P-2,  send own top row
            //For process 1,2,...,P-2,  send own bottom row 
            //For process P-1,          send own top row 
            if(0 == rank) {
                #ifdef DEBUG  
                  printf("%d sending %d to %d with tag %d\n", rank,bottom_row_of_sender, rank+1, SENDING_DOWN);
                #endif 
                MPI_Isend( a[bottom_row_of_sender], n + 2, MPI_DOUBLE, rank + 1, SENDING_DOWN, MPI_COMM_WORLD, &reqs[0] );
            } else if ((numprocessors - 1) == rank) {
                #ifdef DEBUG  
                  printf("%d sending %d to %d with tag %d\n", rank, top_row_of_sender,  rank-1, SENDING_UP);
                #endif
                MPI_Isend( a[top_row_of_sender], n + 2, MPI_DOUBLE, rank - 1, SENDING_UP , MPI_COMM_WORLD, &reqs[0]);        
            } else {
                #ifdef DEBUG  
                  printf("%d sending %d to %d with tag %d\n", rank, top_row_of_sender,  rank-1, SENDING_UP);
                #endif
                MPI_Isend( a[top_row_of_sender], n + 2, MPI_DOUBLE, rank - 1, SENDING_UP , MPI_COMM_WORLD, &reqs[0]);        
                #ifdef DEBUG  
                  printf("%d sending %d to %d with tag %d\n", rank,bottom_row_of_sender, rank+1, SENDING_DOWN);
                #endif 
                MPI_Isend( a[bottom_row_of_sender], n + 2, MPI_DOUBLE, rank + 1, SENDING_DOWN, MPI_COMM_WORLD, &reqs[1] );        

            }

            //For process 0,            receive top row of P1
            //For process 1,2,...,P-2,  receive bottom row of P(rank + 1)
            //For process 1,2,...,P-2,  receive top row of P(rank - 1)
            //For process P-1,          receive bottom row of P-2
            if(0 == rank) {
                #ifdef DEBUG  
                printf("%d receiveing %d from %d with tag %d\n", rank, top_row_rank_plus_one, rank+1, SENDING_UP);
                #endif 
                MPI_Irecv( a[top_row_rank_plus_one],      n + 2, MPI_DOUBLE, rank + 1, SENDING_UP , MPI_COMM_WORLD, &reqs[1]);
            } else if ((numprocessors - 1) == rank) {
                #ifdef DEBUG  
                  printf("%d receiveing %d from %d with tag %d\n", rank, bottom_row_rank_minus_one, rank-1, SENDING_DOWN );
                #endif
                MPI_Irecv( a[bottom_row_rank_minus_one],  n + 2 , MPI_DOUBLE, rank - 1, SENDING_DOWN, MPI_COMM_WORLD, &reqs[1]);
            } else {
                #ifdef DEBUG  
                  printf("%d receiveing %d from %d with tag %d\n", rank, bottom_row_rank_minus_one, rank-1, SENDING_DOWN );
                #endif
                MPI_Irecv( a[bottom_row_rank_minus_one],  n + 2 , MPI_DOUBLE, rank - 1, SENDING_DOWN, MPI_COMM_WORLD, &reqs[2]);
                #ifdef DEBUG  
                printf("%d receiveing %d from %d with tag %d\n", rank, top_row_rank_plus_one, rank+1, SENDING_UP);
                #endif 
                MPI_Irecv( a[top_row_rank_plus_one],      n + 2, MPI_DOUBLE, rank + 1, SENDING_UP , MPI_COMM_WORLD, &reqs[3]);
            }
 
            if(0 != rank && (numprocessors -1) != rank )
                MPI_Waitall(4, reqs, WaitAllStatus);
            else 
                MPI_Waitall(2, reqs, WaitAllStatus);

        }
            i_start = proc_boundary[rank].i_first;
            i_end   = proc_boundary[rank].i_last;
            for(i=i_start;i<i_end;i++) {
                for(j=1;j<n+1;j++) {
                    b[i][j] = 0.2*( a[i][j] + a[i-1][j]  +  a[i+1][j] +  a[i][j-1]  +  a[i][j+1]);
                    if(fabs(b[i][j]-a[i][j])>maxdiff)
                        maxdiff = fabs(b[i][j]-a[i][j]);
                }
            }


        // Copy b to a
        swap_matrix(&a,&b);    

        iteration++;

        MPI_Allreduce( &maxdiff, &gmaxdiff, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD );
    }
    tend = get_clock();
    ttotal = tend-tstart;

    //Query Grid:
    //Only the process containing (r,c) prints out the temperature of the 
    //element with coordinates (r,c). 
    i_start = proc_boundary[rank].i_first;
    i_end = proc_boundary[rank].i_last -1;
 
    if(0 == rank) 
        i_start --;
    if(numprocessors -1 == rank)
        i_end ++;

    if(r >= i_start && r <= i_end) {
        printf("Results:\n");
        printf("Iterations=%d\n",iteration);
        printf("Tolerance=%12.10lf\n",gmaxdiff);
        printf("Running time=%12.8lf\n",ttotal);
        printf("Value at (R,C)=%12.8lf\n",a[r][c]);
    }

    /******************* For Debug Purposes **************************
    ** The code below is to test that the n+2*n+2 array generated
    ** after the MPI communication is the same as that generated in 
    ** the serial code. The easiest way to test is that all the
    ** processes will communicate their rows to process 0, which
    ** will print the entire array, to be matched with the serial
    ** results.
    if(MASTER != rank) {
      int i_start = proc_boundary[rank].i_first;
      int i_end = proc_boundary[rank].i_last -1;
      if(numprocessors - 1 == rank) 
          i_end ++;
      int i;
      for(i = i_start; i <= i_end; i++) {
        #ifdef DEBUG  
          printf("%d sending %d to %d with tag %d\n", rank, i, MASTER, tag);
        #endif 
        MPI_Send( a[i],    n +  2, MPI_DOUBLE, MASTER, rank*10  + i , MPI_COMM_WORLD );        
      }
    }
    if(MASTER == rank) {
        int i,j;
        for(i = 1 ; i < numprocessors; i++) {
            int i_start = proc_boundary[i].i_first;
            int i_end = proc_boundary[i].i_last -1;
            if(numprocessors - 1 == i) 
                i_end ++;
            for(j = i_start; j <= i_end; j++) {
                #ifdef DEBUG  
                  printf("%d receiveing %d from %d with tag %d\n", MASTER, j, i, tag);
               #endif 
                MPI_Recv( a[j],  n + 2, MPI_DOUBLE, i,  i*10 + j , MPI_COMM_WORLD, &status );
            }
        }
    }
    if(MASTER == rank) {
        print_grid(a,0,n+2);
    } 
    ******************************************************************/
  
    MPI_Finalize();

    free_matrix(a,n+2);
    free_matrix(b,n+2);
    return 0;
}
