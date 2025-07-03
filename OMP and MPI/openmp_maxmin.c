#include <omp.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <limits.h>

/**
*	OpenMP Shared Memory Implementation for Finding Maximum and Minimum Values
*
*	This program demonstrates shared memory parallel computing using OpenMP.
*	Multiple threads share the same memory space and coordinate through
*	synchronization mechanisms to find global maximum and minimum values.
*/

void process_dataset_omp(int dataset[], int size, int *max_val, int *min_val, int thread_id, const char* name);

int main() {
// Define three datasets for parallel processing
int dataset1[] = {45, 67, 23, 89, 56, 78, 34, 91, 42, 73};
int dataset2[] = {38, 92, 15, 64, 87, 29, 76, 53, 81, 47};
 
int dataset3[] = {52, 31, 88, 19, 74, 96, 41, 63, 27, 85};
const int data_size = 10;

// Shared variables accessible by all threads int global_max = INT_MIN;
int global_min = INT_MAX;

// Configure number of threads for optimal performance omp_set_num_threads(4);

printf("=== OpenMP SHARED MEMORY PROCESSING ===\n");
printf("Number of threads: %d\n", omp_get_max_threads()); printf("Processing %d elements across 3 datasets\n\n", data_size * 3);

// Parallel region - shared memory approach
// All threads share the same memory space and variables #pragma omp parallel
{
int thread_id = omp_get_thread_num(); int local_max = INT_MIN;
int local_min = INT_MAX;

// Distribute datasets among threads in shared memory environment if (thread_id == 0) {
// Thread 0 processes dataset 1
process_dataset_omp(dataset1, data_size, &local_max, &local_min, thread_id, "Dataset 1");
}
else if (thread_id == 1) {
// Thread 1 processes dataset 2
process_dataset_omp(dataset2, data_size, &local_max, &local_min, thread_id, "Dataset 2");
}
else if (thread_id == 2) {
// Thread 2 processes dataset 3
process_dataset_omp(dataset3, data_size, &local_max, &local_min, thread_id, "Dataset 3");
}
else {
// Additional threads remain idle
printf("Thread %d: No dataset assigned\n", thread_id); local_max = INT_MIN;
local_min = INT_MAX;
}

// Critical section for thread-safe updates to shared variables
// Only one thread can execute this block at a time #pragma omp critical (global_update)
{
// Update global maximum if local value is larger
 
if (local_max > global_max) { global_max = local_max;
}
// Update global minimum if local value is smaller if (local_min < global_min) {
global_min = local_min;
}
}

// Implicit barrier - all threads synchronize at end of parallel region
}

// Display final results (executed by master thread) printf("\n=== RESULTS ===\n");
printf("Global Maximum: %d\n", global_max); printf("Global Minimum: %d\n", global_min); printf("Range: %d\n", global_max - global_min);

return 0;
}

/**
*	Process individual dataset using OpenMP thread
*	Each thread works on its assigned data in shared memory space
*	
*	@param dataset: Array of integers to process
*	@param size: Size of the dataset
*	@param max_val: Pointer to store maximum value
*	@param min_val: Pointer to store minimum value
*	@param thread_id: Thread ID for identification
*	@param name: Dataset name for logging
*/
void process_dataset_omp(int dataset[], int size, int *max_val, int *min_val, int thread_id, const char* name) {
printf("Thread %d: Processing %s\n", thread_id, name);
// Initialize with first element
*max_val = dataset[0];
*min_val = dataset[0];

// Find local extremes in assigned dataset
// This could be further parallelized using OpenMP loop constructs for (int i = 1; i < size; i++) {
if (dataset[i] > *max_val) {
*max_val = dataset[i];
}
if (dataset[i] < *min_val) {
*min_val = dataset[i];
}
}
 
printf("Thread %d: Local Max = %d, Local Min = %d\n", thread_id,
*max_val, *min_val);
}
