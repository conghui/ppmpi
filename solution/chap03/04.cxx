#include <mpi.h>
#include <cstdio>
#include <cstring>
#include <iostream>
/* function prototypes */
void SendMessage(int src, int dst);
void RecvMessage();

int main(int argc, char *argv[])
{
  MPI_Init(&argc, &argv);

  int current_process = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &current_process);

  int total_rank = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &total_rank);

  int id_receiver = (current_process + total_rank - 1) % total_rank;

  // each process p send message to process (p - 1)
  // the first process, whose id is 0 will send message
  // to process to the last process
  SendMessage(current_process, id_receiver);

  // Receive message from anywhere
  RecvMessage();

  MPI_Finalize();
  return 0;
}

void SendMessage(int src, int dst) {
  const int kBufSize    = 100;
  char message[kBufSize];

  std::sprintf(message, "process %d send message to process %d",
               src, dst);
  MPI_Send(message, std::strlen(message) + 1, MPI_CHAR,
           dst, 0, MPI_COMM_WORLD);
}

void RecvMessage() {
  const int kBufSize = 100;
  char message[kBufSize];

  MPI_Status status;
  MPI_Recv(message, kBufSize, MPI_CHAR, MPI_ANY_SOURCE,
           0, MPI_COMM_WORLD, &status);
  std::cerr << message << std::endl;
}
