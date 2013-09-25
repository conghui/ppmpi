#include <iostream>
#include <iomanip>
#include "mpi.h"

int main(int argc, char **argv)
{
  /// set up MPI
  MPI_Init(&argc, &argv);

  int cur_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &cur_rank);

  int num_proc;
  MPI_Comm_size(MPI_COMM_WORLD, &num_proc);

  const int buffer_size = 100;
  char buffer[buffer_size];

  if (cur_rank == 0) {
    // get the number of nonzeros in the row
    // Allocate storage for the row
    // Initialize entries and column_subscripts
    const int nonzeros = 10;
    float *entries = new float[nonzeros];
    int *column_subscripts = new int[nonzeros];
    for (int i = 0; i < nonzeros; i++) {
      entries[i] = 2 * i;
      column_subscripts[i] = 3 * i;
    }

    // now pack the data and send
    int offset = 0;
    MPI_Pack(&nonzeros, 1, MPI_INT, buffer, buffer_size,
             &offset, MPI_COMM_WORLD);
    MPI_Pack(entries, nonzeros, MPI_FLOAT, buffer, buffer_size,
             &offset, MPI_COMM_WORLD);
    MPI_Pack(column_subscripts, nonzeros, MPI_INT, buffer,
             buffer_size, &offset, MPI_COMM_WORLD);

    // send the data
    MPI_Send(buffer, offset, MPI_PACKED, 1, 0, MPI_COMM_WORLD);

    // reclaim the resources
    delete [] entries;
    delete [] column_subscripts;
  } else { /* my_rank == 1 */
    MPI_Status status;
    MPI_Recv(buffer, buffer_size, MPI_PACKED, 0, 0,
             MPI_COMM_WORLD, &status);

    // unpack the data
    int offset = 0;
    int nonzeros;
    MPI_Unpack(buffer, buffer_size, &offset, &nonzeros,
               1, MPI_INT, MPI_COMM_WORLD);

    // allocate storage for entries and column_subscripts
    float *entries           = new float[nonzeros];
    int   *column_subscripts = new int[nonzeros];

    MPI_Unpack(buffer, buffer_size, &offset, entries,
               nonzeros, MPI_FLOAT, MPI_COMM_WORLD);
    MPI_Unpack(buffer, buffer_size, &offset, column_subscripts,
               nonzeros, MPI_INT, MPI_COMM_WORLD);

    // print the sparse
    for (int i = 0; i < nonzeros; i++) {
      std::cout << std::setw(4) << entries[i] <<
        std::setw(4) << column_subscripts[i] << std::endl;
    }

    // reclaim the memory
    delete [] entries;
    delete [] column_subscripts;

  }

  /// shutdown MPI
  MPI_Finalize();
  return 0;
}
