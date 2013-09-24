#include "auxiliary.hxx"

int ceiling_log2(int x) {
  // use unsigned so that right shift will fill leftmost bit with
  // 0
  unsigned temp = static_cast<unsigned>(x) - 1;
  int result = 0;

  while (temp != 0) {
    temp = temp >> 1;
    result = result + 1;
  }

  return result;
}

/**
 * @brief evaluate if the given process should receive data
 *
 * @param stage current stage of distribution
 * @param my_rank current rank of process
 * @param source_ptr if the given process should receive data, that is,
 * return true. |source_ptr| store the rank of source where the data is
 * receive from.
 *
 * @return
 */
bool should_receive(int stage, /* in */
                    int my_rank, /* in */
                    int *source_ptr /* out */)
{
  // 2^stage = 1 << stage
  int power_2_stage = 1 << stage;

  if ((power_2_stage <= my_rank) &&
      (my_rank < 2 * power_2_stage)) {
    *source_ptr = my_rank - power_2_stage;
    return true;
  }

  return false;
}

bool should_send(int stage, /* current stage distribution */
                 int my_rank, /* the rank of this process */
                 int num_proc, /* the total number of process */
                 int *dest_ptr /* output, the dest where the data should send to */)
{

  int power_2_stage = 1 << stage;

  if (my_rank < power_2_stage) {
    *dest_ptr = my_rank + power_2_stage;
    return *dest_ptr >= num_proc ? false : true;
  } else {
    return false;
  }
}

void send_data(float a, /* the lower bound of the interval */
               float b, /* upper bound of the interval */
               int n, /* the total number of splited trapezoid */
               int dest /* destination process */ )
{
  MPI_Send(&a, 1, MPI_FLOAT, dest, 0, MPI_COMM_WORLD);
  MPI_Send(&b, 1, MPI_FLOAT, dest, 1, MPI_COMM_WORLD);
  MPI_Send(&n, 1, MPI_INT,   dest, 2, MPI_COMM_WORLD);
}

void receive_data(float *a, /* lower bound of the interval */
                  float *b, /* upper bound of the interval */
                  int   *n, /* total number of splited trapezoid */
                  int source /* source process*/ )
{
  MPI_Status status;

  MPI_Recv(a, 1, MPI_FLOAT, source, 0, MPI_COMM_WORLD, &status);
  MPI_Recv(b, 1, MPI_FLOAT, source, 1, MPI_COMM_WORLD, &status);
  MPI_Recv(n, 1, MPI_INT,   source, 2, MPI_COMM_WORLD, &status);
}
