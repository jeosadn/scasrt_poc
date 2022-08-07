all : main.c irq.c irq.h bootstrap.S
	riscv32-unknown-elf-gcc -DITER=$(ITER) main.c irq.c bootstrap.S -o main -march=rv32imac -mabi=ilp32 -nostartfiles -Wl,--no-relax

prof:
	rm -rf gmon.out prof.results
	riscv-vp --intercept-syscalls main
	gprof ../../vp/build/bin/riscv-vp gmon.out > prof.results
	
sim: all
	riscv-vp-clean --intercept-syscalls main
	
asrt: all
	riscv-vp-asrt --intercept-syscalls main

fast: all
	riscv-vp-fast --intercept-syscalls main
	
bad: all
	riscv-vp-bad --intercept-syscalls main
	
dump-elf: all
	riscv32-unknown-elf-readelf -a main
	
dump-code: all
	riscv32-unknown-elf-objdump -D main
	
dump-comment: all
	objdump -s --section .comment main
	
clean:
	rm -f main *.log *.out
