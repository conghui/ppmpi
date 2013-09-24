#include <iostream>
#include "mpi.h"


void read_vector_order(const int cur_rank, int *vec_order)
{
  if (cur_rank == 0) {
    std::cin >> *vec_order;
  }

  // broadcast |vec_order| to all process
  int root_id = 0;
  MPI_Bcast(vec_order, 1, MPI_INT, root_id, MPI_COMM_WORLD);
}


template <typename T>
void read_n_value(T *vec, const int n)
{
    for (int i = 0; i < n; i++) {
      std::cin >> vec[i];
    }
}
void read_vector(float *local_vec,
                 int local_n,
                 int total_rank,
                 int cur_rank)
{
  float *temp_vec = new float[local_n];

  if (0 == cur_rank) {
    // read data for process 0
    read_n_value(local_vec, local_n);

    // read data that is needed for other process
    for (int rank = 1; rank < total_rank; rank++) {
      read_n_value(temp_vec, local_n);
      MPI_Send(temp_vec, local_n, MPI_FLOAT, rank,
               0, MPI_COMM_WORLD);
    }
  } else {
    MPI_Status status;
    int root_id = 0;
    MPI_Recv(local_vec, local_n, MPI_FLOAT, root_id,
             0, MPI_COMM_WORLD, &status);
  }
}


float serial_dot(const float *x, /* input array */ 
                 const float *y, /* input array */ 
                 const int n /* # of elements */ )
{
  float sum = 0;

  for (int i = 0; i < n; i++) {
    sum += x[i] * y[i];
  }

  return sum;
}

float parallel_dot(const float *local_xvec,
                   const float *local_yvec,
                   const int local_n)
{
  // perform serial dot product for each local array
  float sub_dot = serial_dot(local_xvec, local_yvec, local_n);

  // reduce all sub-dot-product
  float all_dot;
  MPI_Allreduce(&sub_dot, &all_dot, 1, MPI_FLOAT, 
                MPI_SUM, MPI_COMM_WORLD);

  return all_dot;
}

void print_result(float dot, /* input, dot product */ 
                  int cur_rank,
                  int total_rank)
{
  if (cur_rank == 0) {
    // print process 0
    std::cout << "dot = \n";
    std::cout << "process 0 > " << dot << std::endl;

    // print other processes
    for (int rank_id = 1; rank_id < total_rank; rank_id++) {
      float temp;
      MPI_Status status;
      MPI_Recv(&temp, 1, MPI_FLOAT, rank_id, 0,
               MPI_COMM_WORLD, &status);
      std::cout << "process " << rank_id << " > " 
                << temp << std::endl;
    }
  } else {
    MPI_Send(&dot, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
  }
}

int main(int argc, char **argv)
{
  // set up MPI
  MPI_Init(&argc, &argv);

  int cur_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &cur_rank);

  int total_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &total_rank);

  int vec_order;
  read_vector_order(cur_rank, &vec_order);

  // read data into local arrays
  int local_n = vec_order / total_rank;
  float *local_x = new float[local_n];
  float *local_y = new float[local_n];
  read_vector(local_x, local_n, total_rank, cur_rank);
  read_vector(local_y, local_n, total_rank, cur_rank);

  // calculate the dot product in parallel
  float total_dot = parallel_dot(local_x, local_y, local_n);

  // print the result
  print_result(total_dot, cur_rank, total_rank);

  // claim the resources
  delete [] local_x;
  delete [] local_y;

  // shotdown MPI
  MPI_Finalize();
  return 0;
}
