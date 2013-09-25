/**
 * @file count.cxx
 * @brief send a subvector from process 0 to process 1
 * @note  program should only be run with 2 processes
 * @author Conghui He
 * @version 0.1
 * @date 2013-09-25
 */

#include <algorithm>
#include <iostream>
#include <iterator>

#include "mpi.h"

void mpi_init_var(int *argc, char ***argv,
                  int *cur_rank,
                  int *num_proc)
{
  MPI_Init(argc, argv);
  MPI_Comm_rank(MPI_COMM_WORLD, cur_rank);
  MPI_Comm_size(MPI_COMM_WORLD, num_proc);
}

void print_vec(float *vec, int size,
               const int cur_rank, const int which)
{
  if (cur_rank == which) {
    std::copy(vec, vec + size,
              std::ostream_iterator<float>
              (std::cout, " "));
    std::cout << "\n" << std::endl;
  }
}

int main(int argc, char **argv)
{
  /* set up MPI */
  int cur_rank;
  int num_proc;
  mpi_init_var(&argc, &argv, &cur_rank, &num_proc);

  /* Init the vector and send the second half of vector to process 1 */
  const int vec_size = 100;
  float vector[vec_size]  = {0};
  const int tag = 0;
  if (cur_rank == 0) {
    std::fill(vector + 50, vector + vec_size, 1);

    /* send the second half to process 1 */
    const int recv_id = 1;
    MPI_Send(vector + 50, 50, MPI_FLOAT, recv_id,
             tag, MPI_COMM_WORLD);
  } else { /* cur_rank == 1 */
    /* receive the data send from process 0 */
    const int send_id = 0;
    MPI_Status status;
    MPI_Recv(vector + 50, 50, MPI_FLOAT, send_id,
             tag, MPI_COMM_WORLD, &status);
    /* print the vector by process 1 */
    print_vec(vector + 50, 50, cur_rank, 1);
  }

  /* Shutdown MPI */
  MPI_Finalize();
  return 0;
}
