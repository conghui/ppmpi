/**
 * @file send_col.cxx
 * @brief send the third column of a matrix from process 0 to process 
 * 1
 *
 * @note This program should only be run with 2 processes
 * @author Conghui He
 * @version 0.1
 * @date 2013-09-25
 */

#include <iostream>
#include <iomanip>
#include "mpi.h"

int main(int argc, char **argv)
{
  /// set up MPI
  MPI_Init(&argc, &argv);

  int cur_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &cur_rank);

  /// commit a new type "MPI_Type_vector"
  MPI_Datatype col_mpi_t;
  MPI_Type_vector(10, 1, 10, MPI_FLOAT, &col_mpi_t);
  MPI_Type_commit(&col_mpi_t);

  const int size = 10;
  float A[size][size] = {{0,0}};

  if (cur_rank == 0) {
    /// change the value of elements in the 3rd column
    for (int r = 0; r < size; r++) {
      for (int c = 0; c < size; c++) {
        A[r][c] = c;
      }
    }

    /// send the column to process 1
    const int recv_id = 1;
    MPI_Send(&A[0][2], 1, col_mpi_t, recv_id,
             0, MPI_COMM_WORLD);
  } else {
    /// receive the column from process 0
    MPI_Status status;
    const int send_id = 0;
    MPI_Recv(&A[0][2], 1, col_mpi_t, send_id,
             0, MPI_COMM_WORLD, &status);

    /// print out the column
    for (int r = 0; r < size; r++) {
      std::cout << std::setw(4) << A[r][2];
    }
    std::cout << std::endl;
  }

  /// shutdown MPI
  MPI_Finalize();
}
