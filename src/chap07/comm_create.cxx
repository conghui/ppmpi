/**
 * @file comm_create.cxx
 * @brief build a communicator from the first q processes
 * in a communicator containing |num_proc| = |q|^2 processes
 *
 * Input: none
 *
 * Output: q program tests correct creation of new communicator by
 * broadcasting the value 1 to its members. all other processes have the
 * value 0. global sum computed across all the processes
 *
 * @note Assumes that MPI_COMM_WORLD contains q^2 processes
 *
 * @author Conghui He
 * @version 0.1
 * @date 2013-09-28
 */

#include "mpi.h"
#include <cmath>
#include <cstdio>

int main(int argc, char **argv)
{
  /// set up MPI
  MPI_Init(&argc, &argv);

  int num_proc;
  MPI_Comm_size(MPI_COMM_WORLD, &num_proc);

  int cur_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &cur_rank);

  /// build a new communicator
  // 1. get the group underlying a communicator that contains the process
  //    we want in the new communicator
  MPI_Group group_world;
  MPI_Comm_group(MPI_COMM_WORLD, &group_world);

  // 2. create and init the |ranks_in_old_group| list, and create the new
  //    group from old group
  const int q = std::sqrt(num_proc);
  int *process_ranks = new int[q];
  MPI_Group first_row_group;

  for (int proc = 0; proc < q; proc++) {
    process_ranks[proc] = proc;
  }
  MPI_Group_incl(group_world, q, process_ranks,
                 &first_row_group);

  // 3. create new communicator with the new group
  MPI_Comm first_row_comm;
  MPI_Comm_create(MPI_COMM_WORLD, first_row_group,
                  &first_row_comm);

  /// set a variable to 1 in the new communicator
  int var = 0;
  if (cur_rank < q) {
    int cur_rank_in_first_row;
    MPI_Comm_rank(first_row_comm, &cur_rank_in_first_row);

    if (cur_rank_in_first_row == 0) {
      var = 1;
    }
    MPI_Bcast(&var, 1, MPI_INT, 0, first_row_comm);
  }

  /// compute global sum across all processes
  int sum;
  MPI_Reduce(&var, &sum, 1, MPI_INT, MPI_SUM, 0,
             MPI_COMM_WORLD);

  /// print the necessary information
  if (cur_rank == 0) {
    std::printf("q = %d, sum = %d\n", q, sum);
  }

  /// shutdown mpi
  MPI_Finalize();

  return 0;
}
