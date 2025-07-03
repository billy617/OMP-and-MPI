#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/**
 * MPI Message Passing Implementation for Finding Maximum and Minimum Values
 * 
 * This program demonstrates distributed memory parallel computing using MPI.
 * Each process handles a separate dataset and communicates results through
 * message passing to find global maximum and minimum values.
 */

void process_dataset(int dataset[], int size, int *max_val, int *min_val, int rank, const char* name);

int main(int argc, char** argv) {
    // Initialize MPI environment
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Get process ID
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Get total processes
    
    // Define three datasets for parallel processing
    int dataset1[] = {45, 67, 23, 89, 56, 78, 34, 91, 42, 73};
    int dataset2[] = {38, 92, 15, 64, 87, 29, 76, 53, 81, 47};
    int dataset3[] = {52, 31, 88, 19, 74, 96, 41, 63, 27, 85};
    const int data_size = 10;
    
    // Local results for each process
    int local_max = INT_MIN, local_min = INT_MAX;
    int global_max, global_min;
    
    // Master process displays initial information
    if (rank == 0) {
        printf("=== MPI PARALLEL PROCESSING ===\n");
        printf("Number of processes: %d\n", size);
        printf("Processing %d elements across 3 datasets\n\n", data_size * 3);
    }
    
    // Distribute datasets among processes using message passing approach
    if (rank == 0) {
        // Process 0 handles dataset 1
        process_dataset(dataset1, data_size, &local_max, &local_min, rank, "Dataset 1");
    } 
    else if (rank == 1 && size > 1) {
        // Process 1 handles dataset 2
        process_dataset(dataset2, data_size, &local_max, &local_min, rank, "Dataset 2");
    } 
    else if (rank == 2 && size > 2) {
        // Process 2 handles dataset 3
        process_dataset(dataset3, data_size, &local_max, &local_min, rank, "Dataset 3");
    } 
    else {
        // Additional processes remain idle
        printf("Process %d: No dataset assigned\n", rank);
        local_max = INT_MIN;
        local_min = INT_MAX;
    }
    
    // Synchronize all processes before reduction
    MPI_Barrier(MPI_COMM_WORLD);
    
    // Global reduction operations - core of message passing paradigm
    // All processes send their local results to process 0
    MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_min, &global_min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    
    // Master process displays final results
    if (rank == 0) {
        printf("\n=== RESULTS ===\n");
        printf("Global Maximum: %d\n", global_max);
        printf("Global Minimum: %d\n", global_min);
        printf("Range: %d\n", global_max - global_min);
    }
    
    // Cleanup MPI environment
    MPI_Finalize();
    return 0;
}

/**
 * Process individual dataset to find local maximum and minimum
 * Each process works independently on its assigned data
 * 
 * @param dataset: Array of integers to process
 * @param size: Size of the dataset
 * @param max_val: Pointer to store maximum value
 * @param min_val: Pointer to store minimum value
 * @param rank: Process rank for identification
 * @param name: Dataset name for logging
 */
void process_dataset(int dataset[], int size, int *max_val, int *min_val, int rank, const char* name) {
    printf("Process %d: Processing %s\n", rank, name);
    
    // Initialize with first element
    *max_val = dataset[0];
    *min_val = dataset[0];
    
    // Find local extremes in assigned dataset
    for (int i = 1; i < size; i++) {
        if (dataset[i] > *max_val) {
            *max_val = dataset[i];
        }
        if (dataset[i] < *min_val) {
            *min_val = dataset[i];
        }
    }
    
    printf("Process %d: Local Max = %d, Local Min = %d\n", rank, *max_val, *min_val);
}
