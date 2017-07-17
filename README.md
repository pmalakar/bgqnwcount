# bgqnwcount
Collect network counter information on IBM BG/Q

This code is used for collecting network counters "PEVT_NW_SYS_PP_SENT" and "PEVT_NW_SYS_PP_RECV" on BG/Q. This code was developed in part by using ALCF resources, and is originally used in the paper "Data movement optimizations for independent MPI I/O on the Blue Gene/Q. Parallel Computing 61: 35-51 (2017) by P. Malakar et al."

The Makefile in this directory can be used to compile any code that wishes to use the functions in the file nwcnt.h
 
An example code is provided in multisend.cxx
