#echo "1 10 100" | mpiexec -np 4 ./trap_area
#echo
#echo "1 10 100" | mpiexec -np 4 ./trap_area_reduce
#cat vector_data.in | ./serial_dot
#echo
#cat vector_data.in | mpiexec -np 4 ./parallel_dot
#echo
#cat vector_data.in | mpiexec -np 4 ./parallel_dot_allreduce
#echo

#cat mat_vec.in | ./serial_mat_vec

echo 
cat mat_vec.in | ./parallel_mat_vec
