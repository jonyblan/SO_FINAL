include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_C=$(wildcard c/*.c)
SOURCES_C+=$(wildcard c/Process/*.c)
SOURCES_C+=$(wildcard c/Memory/*.c)
SOURCES_C+=$(wildcard c/Sem/*.c)
SOURCES_DRIVERS=$(wildcard c/drivers/*.c)
SOURCES_INTERRUPTIONS=$(wildcard c/interruptions/*.c)

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_C=$(SOURCES_C:.c=.o)
OBJECTS_DRIVERS=$(SOURCES_DRIVERS:.c=.o)
OBJECTS_INTERRUPTIONS=$(SOURCES_INTERRUPTIONS:.c=.o)

LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) $(OBJECTS_C) $(OBJECTS_DRIVERS) $(OBJECTS_INTERRUPTIONS)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_C) $(OBJECTS_DRIVERS) $(OBJECTS_INTERRUPTIONS) $(STATICLIBS)
	$(LD) $(LDFLAGS) -T kernel.ld  --oformat=elf64-x86-64 -o kernel.elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_C) $(OBJECTS_DRIVERS) $(OBJECTS_INTERRUPTIONS) $(STATICLIBS)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin c/*.o c/drivers/*.o c/interruptions/*.o c/Process/*.o c/Memory/*.o c/Sem/*.o 

.PHONY: all clean