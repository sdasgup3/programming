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
#define MASTER 0                 /* taskid of master */
#define SENDING_LEFT   0           /* setting a message tag */
#define SENDING_RIGHT 1           /* setting a message tag */
#define MASTER 0 

typedef struct process_boundaries {
    int j_first;   
    int j_last;   
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

    int rank, numprocessors, j_start, j_end;
    int leftmost_colm_of_sender, leftmost_colm_rank_plus_one, rightmost_colm_rank_minus_one, rightmost_colm_of_sender; 

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
    double send_buffer1[n+2];
    double send_buffer2[n+2];
    double recv_buffer1[n+2];
    double recv_buffer2[n+2];

    //We assumed that N is evenly divisible by the number of processors P.
    if (0 != n % numprocessors) {
        printf("It is assumed that N is evenly divisible by the number of processors P. Quitting...\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
        exit(1);
    }

    //Partitioning the columns among the diffrent processes
    process_boundary *proc_boundary = (process_boundary *) malloc(numprocessors*sizeof(process_boundary));

    int clmn_partition = n/numprocessors;

    if (0 == clmn_partition) {
        printf("It is assumed that N is greater than number of processors P. Quitting...\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
        exit(1);
    }

    for(j=0; j<numprocessors; j++) {
        proc_boundary[j].j_first = clmn_partition*j + 1 ;
        proc_boundary[j].j_last  = proc_boundary[j].j_first + clmn_partition;
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

        //Top taken care in portions by corresponding process  
        j_start = proc_boundary[rank].j_first;
        j_end   = proc_boundary[rank].j_last;
        for(j = j_start ; j < j_end; j ++) {
          a[0][j] = a[1][j];
        }

        //Bottom taken care in portions by corresponding process  
        j_start = proc_boundary[rank].j_first;
        j_end   = proc_boundary[rank].j_last;
        if(j_start < bclength) {
            if(j_end > bclength) {
                j_end = bclength;
            } 
            for(j = j_start ; j < j_end; j ++) {
              // Bottom
              a[n+1][j] = a[n][j];
            }
        }

        //Left, Bottom Left and Top Left taken care by rank 0
        if(0 == rank) { 
          //Bottom Left
          a[n+1][0] =  a[n][0];

          // Left
          for(i=bclength; i<n+2; i++)
              a[i][0] = a[i][1];

          // Top Left
          a[0][0] = a[1][0];
        } 

        //Top and Top right taken care by rank numprocessors -1
        if((numprocessors - 1 ) == rank) {
          //Top Right
          a[0][n+1] = a[1][n+1];

          // Right
          for(i=0; i<n+2; i++)
              a[i][n+1] = a[i][n];

        }  
        
 
        //For a process of rank 'rank', the columns to send are leftmost  and the rightmost
        leftmost_colm_of_sender     =  proc_boundary[rank].j_first;
        rightmost_colm_of_sender    =  proc_boundary[rank].j_last  -  1;

        //For a process of rank 'rank', the columns to receive are rightmost column of process rank - 1 and the leftmost column
        //of process rank + 1 
        rightmost_colm_rank_minus_one =  proc_boundary[rank-1].j_last - 1 ;
        leftmost_colm_rank_plus_one   =  proc_boundary[rank+1].j_first;
    
        int is_only_one_proc = (0 == numprocessors - 1 );
        maxdiff = 0.0;

        if(0 == is_only_one_proc) {
            //For process 0,            send own rightmost column 
            //For process 1,2,...,P-2,  send own rightmost column
            //For process 1,2,...,P-2,  send own leftmost column 
            //For process P-1,          send own leftmost column 
            if(0 == rank) {
                for(i = 0 ; i < n+2 ; i++) {
                    send_buffer1[i] = a[i][rightmost_colm_of_sender];
                }
                MPI_Isend( send_buffer1, n+2, MPI_DOUBLE, rank + 1, SENDING_RIGHT, MPI_COMM_WORLD, &reqs[0] );        
            } else if((numprocessors - 1) == rank) {
                for(i = 0 ; i < n+2 ; i++) {
                    send_buffer1[i] = a[i][leftmost_colm_of_sender];
                }
                MPI_Isend( send_buffer1, n+2, MPI_DOUBLE, rank - 1, SENDING_LEFT, MPI_COMM_WORLD, &reqs[0] );
            } else {
                for(i = 0 ; i < n+2 ; i++) {
                    send_buffer1[i] = a[i][leftmost_colm_of_sender];
                }
                MPI_Isend( send_buffer1, n+2, MPI_DOUBLE, rank - 1, SENDING_LEFT, MPI_COMM_WORLD, &reqs[0] );
                for(i = 0 ; i < n+2 ; i++) {
                    send_buffer2[i] = a[i][rightmost_colm_of_sender];
                }
                MPI_Isend( send_buffer2, n+2, MPI_DOUBLE, rank + 1, SENDING_RIGHT, MPI_COMM_WORLD, &reqs[1] );        
            } 

            //For process 0,            receive leftmost column of P1
            //For process 1,2,...,P-2,  receive leftmost column of P(rank + 1)
            //For process 1,2,...,P-2,  receive rightmost column of P(rank - 1)
            //For process P-1,          receive rightmost column of P-2
            if(0 == rank) {
                MPI_Irecv( recv_buffer1,  n+2 , MPI_DOUBLE, rank + 1, SENDING_LEFT, MPI_COMM_WORLD, &reqs[1] );
            } else if((numprocessors - 1) == rank) {
                MPI_Irecv( recv_buffer1,  n+2 , MPI_DOUBLE, rank - 1, SENDING_RIGHT, MPI_COMM_WORLD, &reqs[1] );
            } else {
                MPI_Irecv( recv_buffer1,  n+2 , MPI_DOUBLE, rank - 1, SENDING_RIGHT, MPI_COMM_WORLD, &reqs[2] );
                MPI_Irecv( recv_buffer2,  n+2 , MPI_DOUBLE, rank + 1, SENDING_LEFT, MPI_COMM_WORLD, &reqs[3] );
            } 

            // Do Jacobi iteration on the interior of each block
            j_start = proc_boundary[rank].j_first;
            j_end   = proc_boundary[rank].j_last;
            if(0 != rank )
                j_start ++;
            if((numprocessors -1) != rank )
                j_end --;
            for(i = 1;i < n+1; i++) {
                for(j = j_start; j < j_end; j++) {
                    b[i][j] = 0.2*( a[i][j] + a[i-1][j]  +  a[i+1][j] +  a[i][j-1]  +  a[i][j+1]);
                    if(fabs(b[i][j]-a[i][j])>maxdiff)
                        maxdiff = fabs(b[i][j]-a[i][j]);
                }
            } 
         
            if(0 != rank && (numprocessors -1) != rank )
                MPI_Waitall(4, reqs, WaitAllStatus);
            else
                MPI_Waitall(2, reqs, WaitAllStatus);

            if(0 == rank) {
                for(i = 0 ; i < n+2 ; i++) {
                    a[i][leftmost_colm_rank_plus_one] =  recv_buffer1[i];
                }
            } else if((numprocessors - 1) == rank) {
                for(i = 0 ; i < n+2 ; i++) {
                    a[i][rightmost_colm_rank_minus_one] =  recv_buffer1[i];
                }
            } else {
                for(i = 0 ; i < n+2 ; i++) {
                    a[i][rightmost_colm_rank_minus_one] =  recv_buffer1[i];
                }
                for(i = 0 ; i < n+2 ; i++) {
                    a[i][leftmost_colm_rank_plus_one] =  recv_buffer2[i];
                }
            } 

            // Do Jacobi iteration along the perimeter of each block
            if(0 != rank) {
                j_start = proc_boundary[rank].j_first;
                for(i=1;i<n+1;i++) {
                    b[i][j_start] = 0.2*( a[i][j_start] + a[i-1][j_start]  +  a[i+1][j_start] +  a[i][j_start-1]  +  a[i][j_start+1]);
                    if(fabs(b[i][j_start]-a[i][j_start])>maxdiff)
                        maxdiff = fabs(b[i][j_start]-a[i][j_start]);
                }
            }
            if((numprocessors -1) != rank) {
                j_end   = proc_boundary[rank].j_last;
                j_end --;
                for(i=1;i<n+1;i++) {
                    b[i][j_end] = 0.2*( a[i][j_end] + a[i-1][j_end]  +  a[i+1][j_end] +  a[i][j_end-1]  +  a[i][j_end+1]);
                    if(fabs(b[i][j_end]-a[i][j_end])>maxdiff)
                        maxdiff = fabs(b[i][j_end]-a[i][j_end]);
                }
            }

        } else {
            // Compute new grid values
            j_start = proc_boundary[rank].j_first;
            j_end   = proc_boundary[rank].j_last;
            for(i = 1;i < n+1; i++) {
                for(j = j_start; j < j_end; j++) {
                    b[i][j] = 0.2*( a[i][j] + a[i-1][j]  +  a[i+1][j] +  a[i][j-1]  +  a[i][j+1]);
                    if(fabs(b[i][j]-a[i][j])>maxdiff)
                        maxdiff = fabs(b[i][j]-a[i][j]);
                }
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
    double query_result ;
    j_start = proc_boundary[rank].j_first;
    j_end = proc_boundary[rank].j_last -1;
 
    if(0 == rank) 
        j_start --;
    if(numprocessors -1 == rank)
        j_end ++;

    if(c >= j_start && c <= j_end) {
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
    ** processes will communicate their columns to process 0, which
    ** will print the entire array, to be matched with the serial
    ** results.
    if(MASTER != rank) {
      int j_start = proc_boundary[rank].j_first;
      int j_end = proc_boundary[rank].j_last -1;
      if(numprocessors -1 == rank) 
          j_end ++;
      int i;
      for(i = 0; i < n+2; i++) {
          for(j = j_start; j <= j_end; j++) {
              MPI_Send( &a[i][j], 1   , MPI_DOUBLE, MASTER, rank*10  + i*(n+2) + j , MPI_COMM_WORLD );        
          }
      }
    }

    if(MASTER == rank) {
        int i,j,k;
        for(k = 1 ; k < numprocessors; k++) {
            int j_start = proc_boundary[k].j_first;
            int j_end = proc_boundary[k].j_last -1;
            if(numprocessors -1 == k) 
                j_end ++;
            for(i = 0; i < n+2; i++) {
                for(j = j_start; j <= j_end; j++) {
                    MPI_Recv( &a[i][j],  1, MPI_DOUBLE, k,  k*10 + i*(n+2) + j , MPI_COMM_WORLD, &status );
                }
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
