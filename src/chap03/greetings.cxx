#include <mpi.h>
#include <string>
#include <iostream>

int main(int argc, char *argv[])
{
  int tag = 0; /* tag for messages */

  /* Start up MPI */
  MPI_Init(&argc, &argv);

  /* Find out process rank */
  int my_rank = 0; /* rank of process */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  /* Find out number of process */
  int p = 0; /* number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  if (my_rank != 0) {
    std::string message = "Greetings from process" + 
      std::to_string(my_rank) + "!";
    int dest = 0; /* rank of dest */ 

    /* Use strlen + 1 so that '\0' gets transmitted */
    MPI_Send(message.c_str(),
             message.size() + 1, MPI_CHAR,
             dest, 
             tag, 
             MPI_COMM_WORLD);
  } else { /* my_rank == 0 */
    const int kBufSize = 100;
    char message[kBufSize];
    MPI_Status status; /* return status for receive */
    for (int source = 1; source < p; source++) {
      MPI_Recv(message, kBufSize, MPI_CHAR, source, tag,
          MPI_COMM_WORLD, &status);
      int num_recv = 0;
      MPI_Get_count(&status, MPI_CHAR, &num_recv);
      std::cout << "receive " << num_recv << " char: " << 
        message << "\n";      
    }
  }

  /* Shutdown MPI */
  MPI_Finalize();
  return 0;
}
