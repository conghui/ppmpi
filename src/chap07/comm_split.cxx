/**
 * @file comm_split.cxx
 * @brief build a collection of q communicators using MPI_Comm_split
 *
 * Input: none
 *
 * Output: Result of doing a boardcast across each of the q
 * communicators
 *
 * @note Assumes the number of processes, |nun_proc| = q^2
 * @author Conghui He
 * @version 0.1
 * @date 2013-09-29
 */

#include "mpi.h"
#include <cmath>
#include <cstdio>

int main(int argc, char *argv[])
{
  /// set up MPI
  MPI_Init(&argc, &argv);

  int cur_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &cur_rank);

  int num_proc;
  MPI_Comm_size(MPI_COMM_WORLD, &num_proc);

  /// split the communicator
  MPI_Comm cur_row_comm;
  int q = std::sqrt(num_proc);
  int cur_row = cur_rank / q;
  MPI_Comm_split(MPI_COMM_WORLD, cur_row,
                 cur_rank, &cur_row_comm);

  /// assign var to the value of the row
  int var = 0;
  int cur_rank_in_row;
  MPI_Comm_rank(cur_row_comm, &cur_rank_in_row);
  if (cur_rank_in_row == 0) {
    var = cur_row;
  }

  MPI_Bcast(&var, 1, MPI_INT,
            0, cur_row_comm);

  /// print out the result
  std::printf("Process %2d > cur_row = %d, "
              "cur_rank_in_row = %2d, "
              "var = %2d\n",
              cur_rank, cur_row,
              cur_rank_in_row,
              var);

  /// shutdown MPI
  MPI_Finalize();

  return 0;
}
