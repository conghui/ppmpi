#include <iostream>
#include "../chap04/trapezoid.hxx"
#include "mpi.h"

void cal_dispalcement(MPI_Aint *displacements,
                      float *a_ptr,
                      float *b_ptr,
                      int *n_ptr)
{
  // first element, a, is at displacement 0
  displacements[0] = 0;

  // second element, b, is at dispalce |b_addr - a_addr|
  MPI_Aint start_addr;
  MPI_Aint addr;
  MPI_Address(a_ptr, &start_addr);
  MPI_Address(b_ptr, &addr);
  displacements[1] = addr - start_addr;

  // third element, n, is at displacement |n_addr - a_addr|
  MPI_Address(n_ptr, &addr);
  displacements[2] = addr - start_addr;
}

void build_derived_type(float *a_ptr, /* in */ 
                        float *b_ptr, /* in */ 
                        int *n_ptr, /* in */ 
                        MPI_Datatype *mesg_mpi_t_ptr)
{
  const int count = 3;

  // The number of elements in each "block" of the 
  // new type. For us, 1 each
  int block_length[count] = {1, 1, 1};

  // displacement of each element from start of new type
  // The d_i's.
  // MPI_Aini("address int") is an MPI defined C type
  // usually an int
  MPI_Aint displacements[count];
  cal_dispalcement(displacements, a_ptr, b_ptr, n_ptr);

  // specify the old typelist
  MPI_Datatype old_type_list[count] = {
    MPI_FLOAT, MPI_FLOAT, MPI_INT
  };

  // build the derived datatype
  // we can use both |MPI_Type_struct| 
  // and |MPI_Type_create_struct|, 
  // yet, |MPI_Type_struct| is deprecated
  MPI_Type_create_struct(count, 
                         block_length,
                         displacements,                  
                         old_type_list, 
                         mesg_mpi_t_ptr);

  // commit it -- tell system we'll be using it for 
  // communication
  MPI_Type_commit(mesg_mpi_t_ptr);
}

void get_data3(float *a_ptr,
               float *b_ptr,
               int *n_ptr,
               int cur_rank)
{
  // MPI type corresponding to 3 floats and an int
  MPI_Datatype mesg_mpi_t_ptr; 

  if (cur_rank == 0) {
    std::cout << "Enter a, b, and n\n";
    std::cin >> *a_ptr >> *b_ptr >> *n_ptr;
  }

  // all process should build the new type because 
  // all process should understand the new type
  build_derived_type(a_ptr, b_ptr, n_ptr,
                     &mesg_mpi_t_ptr);

  const int send_id = 0;
  MPI_Bcast(a_ptr, 1, mesg_mpi_t_ptr, 
            send_id, MPI_COMM_WORLD);
}

float f(float x) { return x * x; }

void mpi_init_var(int *argc, 
                  char ***argv,
                  int *cur_rank,
                  int *num_proc)
{
  MPI_Init(argc, argv);
  MPI_Comm_rank(MPI_COMM_WORLD, cur_rank);
  MPI_Comm_size(MPI_COMM_WORLD, num_proc);
}

int main(int argc, char *argv[])
{
  /// set up MPI and variables
  int cur_rank;
  int num_proc;
  mpi_init_var(&argc, &argv, &cur_rank, &num_proc);

  /// get data for each process
  float lower_bound;
  float upper_bound;
  int n_trap;
  get_data3(&lower_bound, &upper_bound,
            &n_trap, cur_rank);

  /// calculate the area of trapezoidal
  float height = (upper_bound - lower_bound) / n_trap;
  float local_n = n_trap / num_proc;
  float local_lb = lower_bound + height * local_n * cur_rank;
  float local_ub = local_lb + height * local_n;
  Trapezoid trap(local_lb, local_ub, local_n, f);
  float area = trap.area();

  /// reduce the sub-area to get the sumation
  float total_area;
  MPI_Reduce(&area, &total_area, 1, MPI_FLOAT, 
             MPI_SUM, 0, MPI_COMM_WORLD);

  /// print out the result in process 0
  if (cur_rank == 0) {
    std::cout << "With n = " << n_trap << " trapezoid"
      ", our estimation of the integral from " << 
      lower_bound << " to " << upper_bound << " = " <<
      total_area << std::endl;
  }

  /// reclaim resources and shutdown MPI
  MPI_Finalize();

  return 0;
}
