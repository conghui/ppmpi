#include "get_data.hxx"

void get_data1(float *a, /* lower bound */
              float *b, /* upper bound */
              int   *n_ptr, /* total number of splits */
              int my_rank, /* current rank */
              int num_proc /* number of process */ )
{
  if (my_rank == 0) {
    fprintf(stdout, "Enter a, b and n\n");
    fscanf(stdin, "%f%f%d", a, b, n_ptr);
  }
  for (int stage = 0; stage < ceiling_log2(num_proc); stage++) {
    int source = 0;
    int dest = 0;
    if (should_receive(stage, my_rank, &source)) {
      receive_data(a, b, n_ptr, source);
    } else if (should_send(stage, my_rank, *n_ptr, &dest)) {
      send_data(*a, *b, *n_ptr, dest);
    }
  }
}

void get_data2(float *a, /* lower bound */
               float *b, /* upper bound */
               int *n, /* number of trapezoidal */
               int my_rank, /* current process rank */
               int num_proc /* number of process */ )
{
  if (my_rank == 0) {
    std::cout << "Enter a, b and n" << std::endl;
    std::cin >> *a >> *b >> *n;
  }

  MPI_Bcast(a, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
  MPI_Bcast(b, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
  MPI_Bcast(n, 1, MPI_INT, 0, MPI_COMM_WORLD);

  (void) num_proc; // squeeze compiler complain
}
