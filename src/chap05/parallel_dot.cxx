#include <string>
#include <iostream>

#include "mpi.h"

void read_vector(const std::string &prompt, /* input  */
                 float *local_v, /* out */
                 int local_n, /* in */
                 int total_rank, /* in */
                 int my_rank /* in */ )
{
  float *temp_vec = new float[local_n];

  if (my_rank == 0) {
    std::cout << prompt << std::endl;
    for (int i = 0; i < local_n; i++) {
      std::cin >> local_v[i];
    }

    for (int rank_id = 1; rank_id < total_rank; rank_id++) {
      for (int i = 0; i < local_n; i++) {
        std::cin >> temp_vec[i];
      }

      MPI_Send(temp_vec, local_n, MPI_FLOAT, rank_id,
               0, MPI_COMM_WORLD);
    }
  } else {
    MPI_Status status;
    MPI_Recv(local_v, local_n, MPI_FLOAT, 0, 0,
             MPI_COMM_WORLD, &status);
  }


  delete [] temp_vec;
}

float serial_dot(const float *x,
                 const float *y,
                 const int n)
{
  float sum = 0;
  for (int i = 0; i < n; i++) {
    sum += x[i] * y[i];
  }

  return sum;
}

float parallel_dot(const float *local_x, /* in */
                   const float *local_y, /* in */
                   const int local_n /* in */)
{
  float total_dot = 0;
  const int root_rank = 0;
  float local_dot = serial_dot(local_x, local_y, local_n);
  MPI_Reduce(&local_dot, &total_dot, 1, MPI_FLOAT, MPI_SUM,
             root_rank, MPI_COMM_WORLD);

  return total_dot;
}

int main(int argc, char *argv[])
{
  // setup MPI
  MPI_Init(&argc, &argv);

  int total_rank = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &total_rank);

  int cur_rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &cur_rank);

  int n_elements = 0;
  if (cur_rank == 0) {
    std::cout << "Enter the order of the vectors" << std::endl;
    std::cin >> n_elements;
  }

  // broadcast |n_elements| to other process
  int root_process_id = 0;
  MPI_Bcast(&n_elements, 1, MPI_INT,
            root_process_id, MPI_COMM_WORLD);

  // assume that |n_elements| is divided by |total_rank|
  int local_n = n_elements / total_rank;

  float *local_x = new float[local_n];
  float *local_y = new float[local_n];
  read_vector("Enter the first vector: ", local_x,
              local_n, total_rank, cur_rank);
  read_vector("Enter the second vector: ", local_y,
              local_n, total_rank, cur_rank);

  float total_dot = parallel_dot(local_x, local_y, local_n);

  if (cur_rank == 0) {
    std::cout << "The dot product is: " << total_dot << std::endl;
  }

  // claim the resources
  delete [] local_x;
  delete [] local_y;

  // shutdown MPI
  MPI_Finalize();
  return 0;
}
