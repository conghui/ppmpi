bin_PROGRAMS = trap_area trap_area_reduce \
							 serial_dot \
							 parallel_dot parallel_dot_allreduce  \
							 serial_mat_vec parallel_mat_vec

common = ../chap04/trapezoid.hxx ../chap04/trapezoid.cxx \
				 auxiliary.hxx auxiliary.cxx

trap_area_SOURCES = get_data.hxx get_data.cxx \
										trap_area.cxx \
										$(common)

trap_area_reduce_SOURCES = get_data.hxx get_data.cxx \
										trap_area_reduce.cxx \
										$(common)

serial_dot_SOURCES = serial_dot.cxx

parallel_dot_SOURCES = parallel_dot.cxx

parallel_dot_allreduce_SOURCES = parallel_dot_allreduce.cxx

serial_mat_vec_SOURCES = serial_mat_vec.cxx

parallel_mat_vec_SOURCES = parallel_mat_vec.cxx
