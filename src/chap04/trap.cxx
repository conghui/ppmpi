#include <mpi.h>
#include <iostream>
#include "trapezoid.hxx"

/* function prototypes */
float F(float x);

int main(int argc, char *argv[])
{

  MPI_Init(&argc, &argv);

  // get the total rank
  int rank_total = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &rank_total);

  // get the rank of current process
  int rank_cur = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank_cur);

  float     a       = 1.0f;
  float     b       = 10.0f;
  float     n       = 100;
  float     h       = (b - a) / n;
  float     local_n = n / rank_total;
  float     local_a = a + rank_cur *local_n *h;
  float     local_b = local_a + local_n *h;
  int       tag     = 0;
  Trapezoid trap    = Trapezoid(local_a, local_b, local_n, F);

  if (rank_cur == 0) { /* addup process */
    float      area_total = trap.area();
    float      area_cur   = 0;
    MPI_Status status;

    for (int i = 1; i < rank_total; i++) {
      MPI_Recv(&area_cur, 1, MPI_FLOAT, MPI_ANY_SOURCE,
               tag, MPI_COMM_WORLD, &status);
      area_total += area_cur;
    }

    std::cout << "With n = " << n << " trapezoids, our estimate "
      << "of the integral from " << a << " to " << b << " = "
      << area_total << std::endl;

  } else { /* calculate area process */
    int   rank_recv = 0;
    float area      = trap.area();
    MPI_Send(&area, 1, MPI_FLOAT, rank_recv, tag,
             MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}

float F(float x) {
  return x * x;
}
