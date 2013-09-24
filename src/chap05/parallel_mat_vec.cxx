#include <iostream>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include "mpi.h"

/// process 0 read all the elements of the matrix and
/// process 0 scatters the elements to other process
///
/// Input: row of local matrix
///        col of local matrix
///        current process's rank
///        total number of process 
/// 
/// Output: A local array for each process containing the elements
void read_matrix(float *local_matrix, /* out */ 
                 float local_row, /* in */ 
                 float local_col, /* in */ 
                 int cur_rank, /* in */ 
                 int num_proc /* in */ )
{
  const int total_elem = local_col * local_row *  num_proc;
  float *temp_matrix = NULL;

  if (cur_rank == 0) { 
    /// read data from input stream
    
    temp_matrix = new float[total_elem];
    for (int i = 0; i < total_elem; i++) {
      std::cin >> temp_matrix[i];
    }
  }

  /// scatter the data
  const int local_elem = local_col * local_row;
  const int sender_id = 0;
  MPI_Scatter(temp_matrix, local_elem, MPI_FLOAT,
              local_matrix, local_elem, MPI_FLOAT,
              sender_id, MPI_COMM_WORLD);

  if (cur_rank == 0) {
    delete [] temp_matrix;
  }
}

void print_matrix(const float *local_matrix,
                  const int local_row,
                  const int local_col,
                  const int cur_rank,
                  const int num_proc)
{
  const int total_elem = local_row * num_proc * local_col;
  float *temp_matrix = NULL;

  if (cur_rank == 0) {
    temp_matrix = new float[total_elem];
  }
  
  const int recv_id = 0;
  MPI_Gather(local_matrix, local_col * local_row, MPI_FLOAT,
             temp_matrix, local_col * local_row, MPI_FLOAT,
             recv_id, MPI_COMM_WORLD);

  if (cur_rank == 0) {
    for (int i = 0; i < total_elem; i++) {
      std::cout << std::setw(4) << temp_matrix[i];
      if ((i + 1) % local_col == 0) {
        std::cout << "\n";
      }
    }

    std::cout << '\n';
    // reclaim the resource
    delete [] temp_matrix;
  }
}

void read_matrix_dim(int *row, int *col, int cur_rank)
{
  if (cur_rank == 0) {
    std::cin >> *row >> *col;
  }

  const int sender_id = 0;
  MPI_Bcast(row, 1, MPI_INT, sender_id, MPI_COMM_WORLD);
  MPI_Bcast(col, 1, MPI_INT, sender_id, MPI_COMM_WORLD);
}


void read_vector(float *local_vec,
                 const int local_n,
                 const int cur_rank,
                 const int num_proc)
{
  const int total_elem = local_n * num_proc;
  float *temp_vec = NULL;

  if (cur_rank == 0) {
    temp_vec = new float[total_elem];
    for (int i = 0; i < total_elem; i++) {
      std::cin >> temp_vec[i];
    }
  }

  const int sender_id = 0;
  MPI_Scatter(temp_vec, local_n, MPI_FLOAT,
              local_vec, local_n, MPI_FLOAT,
              sender_id, MPI_COMM_WORLD);

  // reclaim the resource
  if (cur_rank == 0) {
    delete [] temp_vec;
  }
}

void print_vector(const float *local_vec,
                  const int local_n,
                  const int cur_rank,
                  const int num_proc)
{
  const int total_elem = local_n * num_proc;
  float *temp_vec = NULL;

  if (cur_rank == 0) {
    temp_vec = new float[total_elem];
  }

  const int recv_id = 0;
  MPI_Gather(local_vec, local_n, MPI_FLOAT,
             temp_vec, local_n, MPI_FLOAT,
             recv_id, MPI_COMM_WORLD);

  if (cur_rank == 0) {
    std::copy(temp_vec, temp_vec + total_elem,
              std::ostream_iterator<float>(std::cout, " "));
    std::cout << '\n';

    // reclaim the resources
    delete [] temp_vec;
  }
}

void parallel_matrix_vector_prod(const float *local_matrix,
                                 const int local_row,
                                 const int local_col,
                                 const float *local_vec,
                                 const int local_n,
                                 float *local_result,
                                 const int cur_rank)
{
  float *temp_vec = NULL;
  if (cur_rank == 0) {
    temp_vec = new float[local_col];
  }

  MPI_Allgather(local_vec, local_n, MPI_FLOAT,
                temp_vec, local_n, MPI_FLOAT,
                MPI_COMM_WORLD);

  for (int r = 0; r < local_row; r++) {
    local_result[r] = 0;
    for (int c = 0; c < local_col; c++) {
      local_result[r] += 
        local_matrix[r * local_col + c] * temp_vec[c];
    }
  }

  if (cur_rank == 0) {
    delete [] temp_vec;
  }
}

int main(int argc, char *argv[])
{
  // set up mpi
  MPI_Init(&argc, &argv);

  int cur_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &cur_rank);

  int num_proc;
  MPI_Comm_size(MPI_COMM_WORLD, &num_proc);

  // read the dimension of matrix
  int row, col;
  read_matrix_dim(&row, &col, cur_rank);

  // read the matrix
  int &local_col = col;
  int local_row = row / num_proc;
  const int local_elem = local_row * local_col;
  float *local_matrix = new float[local_elem];
  read_matrix(local_matrix, local_row, local_col,
              cur_rank, num_proc);
   
  // print the matrix
  print_matrix(local_matrix, local_row, local_col,
               cur_rank, num_proc);

  // read the vector
  const int local_n = col / num_proc;
  float *local_vec = new float[local_n];
  read_vector(local_vec, local_n, cur_rank, num_proc);

  // print the vector
  print_vector(local_vec, local_n, cur_rank, num_proc);

  // calculate the product parallelly
  float *result_vec = new float[local_n];
  parallel_matrix_vector_prod(local_matrix, local_row, local_col,
                              local_vec, local_n, result_vec,
                              cur_rank);
  // print the resulting vector
  print_vector(result_vec, local_n, cur_rank, num_proc);

  // reclaim the resources
  delete [] result_vec;
  delete [] local_matrix;
  delete [] local_vec;

  // shutdown mpi
  MPI_Finalize();
  return 0;
}
