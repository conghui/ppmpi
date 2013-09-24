#include <iostream>
#include <map>
#include <string>

#include "mpi.h"
#include "auxiliary.hxx"
#include "get_data.hxx"
#include "../chap04/trapezoid.hxx"


int main(int argc, char **argv)
{
  // let the system do what it needs to start up MPI
  MPI_Init(&argc, &argv);

  int my_rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // get my process rank

  int total_rank = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &total_rank); // total number of process

  float lower_bound; // lower bound
  float upper_bound; // upper bound
  int num_trap; // number of trapezoids

  get_data2(&lower_bound, &upper_bound, &num_trap,
           my_rank, total_rank);


  // |height| and |local_num_trap| is the same for all process
  float height = (upper_bound - lower_bound) / num_trap;
  int local_num_trap = num_trap / total_rank;

  float local_lower_bound = lower_bound + my_rank * local_num_trap * height;
  float local_upper_bound = local_lower_bound + local_num_trap * height;

  Trapezoid trapezoid(local_lower_bound, local_upper_bound,
                      local_num_trap, cal_square);

  float integral = trapezoid.area();

  // Add up the integrals calculated by each process
  float total = 0.0f;
  int tag = 0;
  if (my_rank == 0) {
    total = integral;
    for (int source = 1; source < total_rank; source++) {
      MPI_Status status;
      MPI_Recv(&integral, 1, MPI_FLOAT, source,
               tag, MPI_COMM_WORLD, &status);
      total += integral;
    }
  } else {
    int dest = 0;
    MPI_Send(&integral, 1, MPI_FLOAT, dest,
             tag, MPI_COMM_WORLD);
  }

  // print the result
  if (my_rank == 0) {
    fprintf(stdout, "With n = %d trapezoids, our estimate\n"
            "of the integral from %f to %f = %f\n",
            num_trap, lower_bound, upper_bound, total);
  }

  // shotdown MPI
  MPI_Finalize();
  return 0;
}
