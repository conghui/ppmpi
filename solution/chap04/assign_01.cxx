#include <mpi.h>
#include <iostream>
#include <cmath>
#include <cassert>
#include "../../src/chap04/trapezoid.hxx"

/* function prototypes */
float F1(float x);
float F2(float x);
float F3(float x);
void DisplayMenu();
bool IsValid(int choice);
int GetUserChoice(const int rank_cur, const int rank_total);

typedef float (*func_t)(float);

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
  func_t    funcs[] = {F1, F2, F3}; // array of function pointers
  int       choice  = GetUserChoice(rank_cur, rank_total);
  Trapezoid trap    = Trapezoid(local_a, local_b, local_n, funcs[choice - 1]);

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

float F1(float x) { return x * x;}
float F2(float x) { return x + 1; }
float F3(float x) { return std::sin(x); }

void DisplayMenu() {
  std::cout << "Please select which function you want to use to "
            << "integrate the trapezoid\n"
            << "1. f(x) = x * x\n"
            << "2. f(x) = x + 1\n"
            << "3. f(x) = sin(x)\n"
            << "Enter your choice (1-3): ";
}

bool IsValid(int choice) {
  return choice >= 1 && choice <= 3;
}

int GetUserChoice(const int rank_cur, const int rank_total) {
  int choice = 0;
  const int tag = 0;
  if (rank_cur == 0) {
    do {
      DisplayMenu();
      std::cin >> choice;
    } while (!IsValid(choice));

    for (int dst = 1; dst < rank_total; dst++) {
      MPI_Send(&choice, 1, MPI_INT, dst, tag, MPI_COMM_WORLD);
    }

  } else {
    const int src = 0;
    MPI_Status status;
    MPI_Recv(&choice, 1, MPI_INT, src, tag, MPI_COMM_WORLD, &status);
  }

  return choice;
}
