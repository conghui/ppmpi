#ifndef GET_DATA1_HXX_YXCRFDET
#define GET_DATA1_HXX_YXCRFDET

#include <iostream>
#include "auxiliary.hxx"
/* function prototypes */
void get_data1(float *a, float *b, int *n_ptr, int my_rank, int num_proc );

void get_data2(float *a, /* lower bound */
               float *b, /* upper bound */
               int *n, /* number of trapezoidal */
               int my_rank, /* current process rank */
               int num_proc /* number of process */ );

#endif /* end of include guard: GET_DATA1_HXX_YXCRFDET */
