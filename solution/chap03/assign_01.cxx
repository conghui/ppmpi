#include <mpi.h>
#include <cstdio>
#include <cstring>
#include <iostream>

int main(int argc, char *argv[])
{
  MPI_Init(&argc, &argv);

  int total_rank = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &total_rank);

  int rank_cur = 0; /* current process */
  MPI_Comm_rank(MPI_COMM_WORLD, &rank_cur);

  /* the receiver where the message sent to */
  int rank_recv = (rank_cur + 1) % total_rank;

  const int kBufSize          = 100;
  char      message[kBufSize] = {0};
  std::sprintf(message, "process %d send message to process %d\n",
               rank_cur, rank_recv);
  MPI_Send(message, std::strlen(message) + 1, MPI_CHAR, rank_recv,
           0, MPI_COMM_WORLD);

  // the message has been sent out, now receive the message
  int rank_send = (rank_cur + total_rank - 1) % total_rank;
  MPI_Status status;
  MPI_Recv(message, kBufSize, MPI_CHAR, rank_send,
           0, MPI_COMM_WORLD, &status);

  std::cout << message;

  return 0;
}
