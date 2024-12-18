# Compiler
CC = gcc
CPP = g++ -std=c++20
ASM = nasm

# Fib heap program name
PNAME = fib.o
# Tests generator program name
TNAME = gen.o
# Answer generator program name
ANAME = ans.o

# Flags
CFLAGS = -z noexecstack

# Fast Flags
#CFLAGS += -O3 -DNDEBUG

# Debugging Flags
CFLAGS += -D _DEBUG \
         -ggdb3 \
         -Wall -Wextra \
         -Waggressive-loop-optimizations \
         -Wmissing-declarations -Wcast-align \
         -Wcast-qual -Wchar-subscripts \
         -Wconversion -Wempty-body \
         -Wfloat-equal -Wformat-nonliteral \
         -Wformat-security -Wformat-signedness \
         -Wformat=2 -Winline -Wlogical-op \
         -Wopenmp-simd -Wpacked -Wpointer-arith \
         -Winit-self -Wredundant-decls -Wshadow \
         -Wsign-conversion -Wstrict-overflow=2 \
         -Wsuggest-attribute=noreturn \
         -Wsuggest-final-methods -Wsuggest-final-types \
         -Wswitch-default -Wswitch-enum \
         -Wsync-nand -Wundef -Wunreachable-code \
         -Wunused -Wvariadic-macros \
         -Wno-missing-field-initializers \
         -Wno-narrowing -Wno-varargs \
         -Wstack-protector -fcheck-new \
         -fstack-protector -fstrict-overflow \
         -flto-odr-type-merging \
         -fno-omit-frame-pointer \
         -Wstack-usage=8192 \
         -pie -fPIE -Werror=vla \
         -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

# GDB Flags
GDBFLAGS = -g

# Objects dir
OBJ_DIR = obj

# Sources
SRCS_C = src/FibHeap/FibHeap.c
SRCS_C += src/Stack/Stack.c
SRCS_C += src/Graph/Graph.c
SRCS_ASM = src/ASM_FibHeap/ASM_FibHeap.asm

SRCS_TG_C = src/Testing/TestGenerator/TestGenerator.c

SRCS_CPP = main.cpp
SRCS_CPP += src/Testing/FibHeapTesting/FibHeapTesting.cpp

SRCS_AG_CPP = src/Testing/AnswersGenerator/AnswersGenerator.cpp

# Objects
OBJS_C     = $(SRCS_C:.c=.o)
OBJS_C    += $(SRCS_ASM:.asm=.o)
OBJS_TG_C  = $(SRCS_TG_C:.c=.o)

OBJS_CPP  = $(SRCS_CPP:.cpp=.o)
OBJS_AG_CPP = $(SRCS_AG_CPP:.cpp=.o)

# Testing script
TESTING_SH = src/Testing/testsing.sh

# Optional testing parameters (You can leave it empty)
# -n : number of tests, -s : the size of the tests
OPTIONATESTING_SH_OPTIONAL_PARAMSL = -n 100000 -s 100000 # For example: -n 100000 -s 100

%.o: %.c
	@mkdir -p $(dir $(OBJ_DIR)/$@)
	@$(CC) $(CFLAGS) $(GDBFLAGS) -c $< -o $(OBJ_DIR)/$@ -lm

%.o: %.cpp
	@mkdir -p $(dir $(OBJ_DIR)/$@)
	@$(CPP) $(CFLAGS) $(GDBFLAGS) -c $< -o $(OBJ_DIR)/$@ -lm

%.o: %.asm
	@mkdir -p $(dir $(OBJ_DIR)/$@)
	@$(ASM) -f elf64 $(GDBFLAGS) $< -o $(OBJ_DIR)/$@

compile: $(OBJS_C) $(OBJS_CPP) $(OBJS_TG_C) $(OBJS_AG_CPP)
	@$(CPP) $(CFLAGS) $(patsubst %,obj/%,$(OBJS_C)) $(patsubst %,obj/%,$(OBJS_CPP)) -o $(PNAME)
	@$(CC) $(CFLAGS) $(patsubst %,obj/%,$(OBJS_TG_C)) -o $(TNAME)
	@$(CPP) $(CFLAGS) $(patsubst %,obj/%,$(OBJS_AG_CPP)) -o $(ANAME)

testing: 
	@$(TESTING_SH) -p $(PNAME) -t $(TNAME) -a $(ANAME) $(OPTIONATESTING_SH_OPTIONAL_PARAMSL)

clean:
	@rm -rf $(OBJ_DIR) $(PNAME) $(TNAME) $(ANAME)