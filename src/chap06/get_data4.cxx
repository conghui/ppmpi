#include <iostream>
#include "mpi.h"
#include "../chap04/trapezoid.hxx"

void pack_into(char *buffer,
               const int size,
               float *lower_bound,
               float *upper_bound,
               int *n_trap)
{
  int offset = 0;

  // |offset| is in/out
  MPI_Pack(lower_bound, 1, MPI_FLOAT, buffer, size,
           &offset, MPI_COMM_WORLD);

  // |offset| has been incremented
  MPI_Pack(upper_bound, 1, MPI_FLOAT, buffer, size,
           &offset, MPI_COMM_WORLD);

  // |offset| has been incremented again
  MPI_Pack(n_trap, 1, MPI_INT, buffer, size,
        &offset, MPI_COMM_WORLD);
}

void unpack_from(char *buffer,
                      const int size,
                      float *lower_bound,
                      float *upper_bound,
                      int *n_trap)
{
  int offset = 0;
  MPI_Unpack(buffer, size, &offset, lower_bound,
             1, MPI_FLOAT, MPI_COMM_WORLD);

  // |offset| has been incremented
  MPI_Unpack(buffer, size, &offset, upper_bound,
             1, MPI_FLOAT, MPI_COMM_WORLD);

  // |offset| has been incremented again
  MPI_Unpack(buffer, size, &offset, n_trap,
             1, MPI_INT, MPI_COMM_WORLD);

}
void get_data4(float *lower_bound,
               float *upper_bound,
               int *n_trap,
               int cur_rank)
{
  const int buffer_size = 100;
  char buffer[buffer_size];

  if (cur_rank == 0) {
    /// read arguments from stdin
    std::cout << "Enter a, b and n\n";
    std::cin >> *lower_bound >> *upper_bound >> *n_trap;

    /// pack the data into buffer
    pack_into(buffer, buffer_size,
              lower_bound, upper_bound, n_trap);

    /// boardcast -- send them
    MPI_Bcast(buffer, buffer_size, MPI_PACKED,
              0, MPI_COMM_WORLD);
  } else {
    /// boardcast -- receive them
    MPI_Bcast(buffer, buffer_size, MPI_PACKED,
              0, MPI_COMM_WORLD);

    /// unpack the data from buffer
    unpack_from(buffer, buffer_size,
                lower_bound, upper_bound, n_trap);
  }

}

float f(float x) { return x * x; }

void mpi_init_var(int *argc,
                  char ***argv,
                  int *cur_rank,
                  int *num_proc)
{
  MPI_Init(argc, argv);
  MPI_Comm_rank(MPI_COMM_WORLD, cur_rank);
  MPI_Comm_size(MPI_COMM_WORLD, num_proc);
}

int main(int argc, char *argv[])
{
  /// set up MPI and variables
  int cur_rank;
  int num_proc;
  mpi_init_var(&argc, &argv, &cur_rank, &num_proc);

  /// get data for each process
  float lower_bound;
  float upper_bound;
  int n_trap;
  get_data4(&lower_bound, &upper_bound,
            &n_trap, cur_rank);

  /// calculate the area of trapezoidal
  float height = (upper_bound - lower_bound) / n_trap;
  float local_n = n_trap / num_proc;
  float local_lb = lower_bound + height * local_n * cur_rank;
  float local_ub = local_lb + height * local_n;
  Trapezoid trap(local_lb, local_ub, local_n, f);
  float area = trap.area();

  /// reduce the sub-area to get the sumation
  float total_area;
  MPI_Reduce(&area, &total_area, 1, MPI_FLOAT,
             MPI_SUM, 0, MPI_COMM_WORLD);

  /// print out the result in process 0
  if (cur_rank == 0) {
    std::cout << "With n = " << n_trap << " trapezoid"
      ", our estimation of the integral from " <<
      lower_bound << " to " << upper_bound << " = " <<
      total_area << std::endl;
  }

  /// reclaim resources and shutdown MPI
  MPI_Finalize();

  return 0;
}
