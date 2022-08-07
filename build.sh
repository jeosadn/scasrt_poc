mkdir -p vp/build
cd vp/build
touch ../../vp/src/platform/basic/main.cpp && cmake -DCMAKE_CXX_FLAGS=-pg -DCMAKE_EXE_LINKER_FLAGS=-pg -DCMAKE_SHARED_LINKER_FLAGS=-pg .. && make -j 4
