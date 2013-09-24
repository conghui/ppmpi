#ifndef AUXILIARY_HXX_HSW0X8YM
#define AUXILIARY_HXX_HSW0X8YM

#include "mpi.h"

template <typename T>
T cal_square(T x) { return x * x; }

/**
 * @brief Ceiling of log_2(x) 
 *
 *  It is just the number of times that |x - 1| can be divided by 2 
 *  until the quotient is 0. Dividing by 2 is the same as right shift.
 *
 * @param x input value
 *
 * @return ceiling of log_2(x)
 */
int ceiling_log2(int x);

bool should_receive(int stage, int my_rank, int *source_ptr );
bool should_send(int stage, int my_rank, int num_proc, int *dest_ptr );
void send_data(float a, float b, int n, int dest );
void receive_data(float *a, float *b, int *n, int source );
#endif /* end of include guard: AUXILIARY_HXX_HSW0X8YM */
