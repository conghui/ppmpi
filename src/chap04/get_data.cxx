#include <iostream>
#include <mpi.h>
#include <map>
#include <string>

#include "trapezoid.hxx"

/* function prototypes */
void GetData(float &a, float &b, float &n, const int rank_cur, const int rank_total);
float F(float x);

int main(int argc, char *argv[])
{
  MPI_Init(&argc, &argv);

  int rank_total = 0; /* total # of process */
  int rank_cur   = 0; /* id of current process */
  MPI_Comm_size(MPI_COMM_WORLD, &rank_total);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank_cur);

  float a, b, n;
  GetData(a, b, n, rank_cur, rank_total);

  const float h       = (b - a) / n;
  const float local_n = n / rank_total;
  const float local_a = a + rank_cur *local_n *h;
  const float local_b = local_a + local_n *h;
  const int   tag     = 0;
  Trapezoid   trap    = Trapezoid(local_a, local_b, local_n, F);

  if (rank_cur == 0) {
    float area_total = trap.area();
    float area_cur = 0;

    for (int src = 1; src < rank_total; src++) {
      MPI_Status status;
      MPI_Recv(&area_cur, 1, MPI_FLOAT, MPI_ANY_SOURCE, tag,
               MPI_COMM_WORLD, &status);
      area_total += area_cur;
    }

    std::cout << "With n = " << n << " trapezoids, our estimate "
      << "of the integral from " << a << " to " << b << " = "
      << area_total << std::endl;

  } else { /* send the area of sub-trapezoid to process 0 */
    const int dst = 0;
    float area = trap.area();
    MPI_Send(&area, 1, MPI_FLOAT, dst, tag,
             MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}

void GetData(float &a, float &b, float &n,
             const int rank_cur,
             const int rank_total)
{
  const int source = 0;
  std::map<std::string, int> tags;

  tags["a"] = 0;
  tags["b"] = 1;
  tags["n"] = 2;

  if (rank_cur == source) {
    std::cout << "Enter a, b and n: ";
    std::cin >> a >> b >> n;

    // send data to other process
    for (int dst = 1; dst < rank_total; dst++) {
      MPI_Send(&a, 1, MPI_FLOAT, dst, tags["a"], MPI_COMM_WORLD);
      MPI_Send(&b, 1, MPI_FLOAT, dst, tags["b"], MPI_COMM_WORLD);
      MPI_Send(&n, 1, MPI_FLOAT, dst, tags["n"], MPI_COMM_WORLD);
    }
  } else { /* receiving process */
    MPI_Status status;
    MPI_Recv(&a, 1, MPI_FLOAT, source, tags["a"], MPI_COMM_WORLD, &status);
    MPI_Recv(&b, 1, MPI_FLOAT, source, tags["b"], MPI_COMM_WORLD, &status);
    MPI_Recv(&n, 1, MPI_FLOAT, source, tags["n"], MPI_COMM_WORLD, &status);
  }
}

float F(float x) { return x * x; }
