#!/bin/env bash

export PATH=/home/josea/my_repos/riscv-vp/inst/bin:/home/josea/my_repos/riscv-vp/riscv-vp/vp/build/bin/:$PATH

# To compile with gprof, use the following cmake command:
# cmake -DCMAKE_CXX_FLAGS=-pg -DCMAKE_EXE_LINKER_FLAGS=-pg -DCMAKE_SHARED_LINKER_FLAGS=-pg .. && make -j 4
# the basic-dma test has been modified to generate profiling results
# For memory profiling, zsh provides the time command. Add the following to your zshrc:
#  if [[ `uname` == Darwin ]]; then
#      MAX_MEMORY_UNITS=KB
#  else
#      MAX_MEMORY_UNITS=MB
#  fi
#  
#  TIMEFMT='%J   %U  user %S system %P cpu %*E total'$'\n'\
#  'avg shared (code):         %X KB'$'\n'\
#  'avg unshared (data/stack): %D KB'$'\n'\
#  'total (sum):               %K KB'$'\n'\
#  'max memory:                %M '$MAX_MEMORY_UNITS''$'\n'\
#  'page faults from disk:     %F'$'\n'\
#  'other page faults:         %R'
