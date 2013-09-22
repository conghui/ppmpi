#include <mpi.h>
#include <cstdio>
#include <cstring>
#include <iostream>

int main(int argc, char *argv[])
{
  MPI_Init(&argc, &argv);

  int current_process;
  MPI_Comm_rank(MPI_COMM_WORLD, &current_process);

  const int id_receiver = 0;
  const int tag         = 0;
  const int kBufSize    = 100;
  char message[kBufSize];

  if (current_process == id_receiver) {
    /* receive the message using wildcard */
    int num_process = 0; /* # of process */
    MPI_Comm_size(MPI_COMM_WORLD, &num_process);

    MPI_Status status; /* receive status */

    for (int i = 1; i < num_process; i++) {
      MPI_Recv(message, kBufSize, MPI_CHAR, MPI_ANY_SOURCE,
               tag, MPI_COMM_WORLD, &status);

      std::cout << message;
    }

  } else { /* not receiver */
    std::sprintf(message, "Greeting message from process # %d\n",
                 current_process);

    /* send out the message */
    MPI_Send(message, std::strlen(message) + 1, MPI_CHAR,
             id_receiver, tag, MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}
