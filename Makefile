# Compiler
CC = gcc
CPP = g++
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
         -Wlarger-than=8192 -Wstack-usage=8192 \
         -pie -fPIE -Werror=vla \
         -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

# GDB Flags
GDBFLAGS = -g

# Objects dir
OBJ_DIR = obj

# Sources
SRCS  = main.c
SRCS += src/FibHeap/FibHeap.c
SRCS += src/Stack/Stack.c
SRCS += src/Graph/Graph.c
SRCS += src/Dirs/Dirs.c
SRCS += src/Testing/TestingHeap/TestingHeap.c

SRCS_ASM  = src/ASM_FibHeap/ASM_FibHeap.asm

ANSWER_GENERATOR_SRCS = src/Testing/TestsAnsGenerator/AnsGenerator.cpp
TEST_GENERATOR_SRCS = src/Testing/TestsAnsGenerator/TestGenerator.c

# Objects
OBJS  = $(SRCS:.c=.o)
OBJS += $(SRCS_ASM:.asm=.o)

TEST_GENERATOR_OBJS = $(TEST_GENERATOR_SRCS:.c=.o)
ANSWER_GENERATOR_OBJS = $(ANSWER_GENERATOR_SRCS:.cpp=.o)

# Testing script
TESTING_SH = src/Testing/testsing.sh

# Optional testing parameters (You can leave it empty)
OPTIONATESTING_SH_OPTIONAL_PARAMSL = -n 1000000 -s 1000000 # For example: -n 100000 -s 100

%.o: %.c
	@mkdir -p $(dir $(OBJ_DIR)/$@)
	@$(CC) $(CFLAGS) $(GDBFLAGS) -c $< -o $(OBJ_DIR)/$@ -lm
%.o: %.cpp
	@mkdir -p $(dir $(OBJ_DIR)/$@)
	@$(CPP) $(CFLAGS) $(GDBFLAGS) -c $< -o $(OBJ_DIR)/$@ -lm
%.o: %.asm
	@mkdir -p $(dir $(OBJ_DIR)/$@)
	@$(ASM) -f elf64 $(GDBFLAGS) $< -o $(OBJ_DIR)/$@

compile: $(OBJS) $(TEST_GENERATOR_OBJS) $(ANSWER_GENERATOR_OBJS)
	@$(CC)  $(CFLAGS) $(patsubst %,obj/%,$(OBJS)) 					-o $(PNAME)
	@$(CC)  $(CFLAGS) $(patsubst %,obj/%,$(TEST_GENERATOR_OBJS)) 	-o $(TNAME)
	@$(CPP) $(CFLAGS) $(patsubst %,obj/%,$(ANSWER_GENERATOR_OBJS)) 	-o $(ANAME)

testing: 
	@$(TESTING_SH) -p $(PNAME) -t $(TNAME) -a $(ANAME) $(OPTIONATESTING_SH_OPTIONAL_PARAMSL)

clean:
	@rm -rf $(OBJ_DIR) $(PNAME) $(TNAME) $(ANAME)