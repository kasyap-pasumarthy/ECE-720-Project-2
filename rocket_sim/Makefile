
PROGNAME = lstm
MAXCYCLES = 150000

# The following 9 lines were taken from ${RISCV}/riscv-tests/benchmarks/Makefile
XLEN ?= 64
RISCV_PREFIX ?= riscv$(XLEN)-unknown-elf-
RISCV_GCC ?= $(RISCV_PREFIX)gcc
RISCV_GCC_OPTS ?= -DPREALLOCATE=1 -mcmodel=medany -static -std=gnu99 -O0 -ffast-math -fno-common -fno-builtin-printf
RISCV_LINK ?= $(RISCV_GCC) -T $(RISCV)/riscv-tests/benchmarks/common/test.ld $(incs)
RISCV_LINK_OPTS ?= -static -nostdlib -nostartfiles -lm -lgcc -T $(RISCV)/riscv-tests/benchmarks/common/test.ld
RISCV_OBJDUMP ?= $(RISCV_PREFIX)objdump --disassemble-all --disassemble-zeroes --section=.text --section=.text.startup --section=.data
RISCV_SIM ?= ../systemc/main.x --isa=rv$(XLEN)gc
incs  += -I$(RISCV)/riscv-tests/env -I$(RISCV)/riscv-tests/benchmarks/common


OBJDUMP = riscv64-unknown-elf-objdump
EMULATOR = emulator-freechips.rocketchip.system-freechips.rocketchip.system.DefaultConfig
DISASSEMBLE = spike-dasm


$(PROGNAME).riscv: $(wildcard *.c) $(wildcard *.h) $(wildcard *.S) 
	$(RISCV_GCC) $(incs) $(RISCV_GCC_OPTS) -o $@ $(wildcard *.c) $(wildcard *.S) $(wildcard $(RISCV)/riscv-tests/benchmarks/common/*.c) $(wildcard $(RISCV)/riscv-tests/benchmarks/common/*.S) $(RISCV_LINK_OPTS)
	$(OBJDUMP) -D -S $(PROGNAME).riscv > $(PROGNAME).riscv.dump


sim: $(PROGNAME).riscv
	time $(RISCV_SIM) -l $(PROGNAME).riscv 2> $(PROGNAME).spike.out
	trace -f spike -o $(PROGNAME).riscv.dump $(PROGNAME).spike.out > $(PROGNAME).spike.trace

vsim: $(PROGNAME).riscv
	time $(EMULATOR) +max-cycles=$(MAXCYCLES) +verbose $(PROGNAME).riscv 3>&1 1>&2 2>&3 | $(DISASSEMBLE) > $(PROGNAME).emulator.out
	trace -o $(PROGNAME).riscv.dump $(PROGNAME).emulator.out  > $(PROGNAME).emulator.trace

vcd: $(PROGNAME).vcd

$(PROGNAME).vcd: $(PROGNAME).riscv
	time $(EMULATOR)-debug +max-cycles=$(MAXCYCLES) +verbose -v$(PROGNAME).vcd $(PROGNAME).riscv 3>&1 1>&2 2>&3 | $(DISASSEMBLE) > $(PROGNAME).emulator.out
	trace -o $(PROGNAME).riscv.dump $(PROGNAME).emulator.out  > $(PROGNAME).emulator.trace

vpd: $(PROGNAME).vpd

$(PROGNAME).vpd: $(PROGNAME).vcd
	vcd2vpd $(PROGNAME).vcd $(PROGNAME).vpd

pwr: 
	#sed "/set PROGNAME/c\set PROGNAME $(PROGNAME)" run_ptpx.tcl > tmp
	#mv tmp run_ptpx.tcl
	pt_shell -f run_ptpx.tcl |& tee run_ptpx.log

view: $(PROGNAME).vpd
	dve &

clean:
	-rm -f $(OBJ) $(PROGNAME).riscv $(PROGNAME).riscv.dump 
	-rm -f $(PROGNAME).spike.out $(PROGNAME).emulator.out 
	-rm -f $(PROGNAME).spike.trace $(PROGNAME).emulator.trace 
	-rm -f $(PROGNAME).vcd $(PROGNAME).vpd
	-rm -rf DVEfiles
	-rm -f power_ptpx.rpt pt_shell_command.log run_ptpx.log

