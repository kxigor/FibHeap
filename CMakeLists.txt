#=========================== General Settings ==========================
cmake_minimum_required(VERSION 3.16)

project(FibHeapProject VERSION 0.1 LANGUAGES C CXX ASM_NASM)
enable_language(ASM_NASM)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_EXECUTABLE_SUFFIX ".out")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# TODO: remove in release
# Set the default CMAKE_BUILD_TYPE to Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -z noexecstack")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -z noexecstack")
set(CMAKE_ASM_NASM_FLAGS "${CMAKE_ASM_NASM_FLAGS} -f elf64")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -D_DEBUG -ggdb3 -Waggressive-loop-optimizations -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wempty-body -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wopenmp-simd -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wswitch-default -Wswitch-enum -Wsync-nand -Wunused -Wundef -Wunreachable-code -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-varargs -Wstack-protector -fcheck-new -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -D_DEBUG -ggdb3 -Waggressive-loop-optimizations -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wempty-body -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wopenmp-simd -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wswitch-default -Wswitch-enum -Wsync-nand -Wunused -Wundef -Wunreachable-code -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-varargs -Wstack-protector -fcheck-new -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr")
endif()
#=======================================================================


#=========================== Add executables ===========================
set(SRCS_MAIN
    src/main.cpp
)
set(SRCS_FIBHEAP
    src/FibHeap/FibHeap.c
    src/ASM_FibHeap/ASM_FibHeap.asm
    src/Graph/Graph.c
    src/Stack/Stack.c
    src/Testing/FibHeapTesting/FibHeapTesting.cpp
)
#=======================================================================


#=============== Create libraries from source files ====================
add_library(fibheap STATIC ${SRCS_FIBHEAP})
#=======================================================================


#=============== Create an executable for interfaces ====================
add_executable(asm_fib_heap ${SRCS_MAIN})
target_compile_definitions(asm_fib_heap PUBLIC ASM_FIBHEAP)
target_compile_definitions(asm_fib_heap PUBLIC GRAPHBUILDER=true)
target_link_libraries(asm_fib_heap fibheap)
#=======================================================================


#=============================== Tests =================================
enable_testing()

set(TESTS_GENERATOR_NAME tests_generator)
set(ANSWER_GENERATOR_NAME answer_generator)
set(FIB_HEAP_NAME my_fibheap_answer_generator)
set(TESTING_SH ${CMAKE_SOURCE_DIR}/src/Testing/testsing.sh)
set(SRCS_TESTS_GENERATOR
    src/Testing/TestGenerator/TestGenerator.c
)
set(SRCS_ANSWER_GENERATOR
    src/Testing/AnswersGenerator/AnswersGenerator.cpp
)
add_executable(${TESTS_GENERATOR_NAME} ${SRCS_TESTS_GENERATOR})
set_property(TARGET ${TESTS_GENERATOR_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY "")
add_executable(${ANSWER_GENERATOR_NAME} ${SRCS_ANSWER_GENERATOR})
set_property(TARGET ${ANSWER_GENERATOR_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY "")
add_executable(${FIB_HEAP_NAME} ${SRCS_MAIN})
set_property(TARGET ${FIB_HEAP_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY "")
target_compile_definitions(${FIB_HEAP_NAME} PUBLIC ASM_FIBHEAP)
target_compile_definitions(${FIB_HEAP_NAME} PUBLIC GRAPHBUILDER=false)
target_link_libraries(${FIB_HEAP_NAME} fibheap)

function(add_custom_test test_name num_tests size)
    add_test(
        NAME ${test_name}
        COMMAND ${TESTING_SH} -p ${FIB_HEAP_NAME}${CMAKE_EXECUTABLE_SUFFIX} -t ${TESTS_GENERATOR_NAME}${CMAKE_EXECUTABLE_SUFFIX} -a ${ANSWER_GENERATOR_NAME}${CMAKE_EXECUTABLE_SUFFIX} -n ${num_tests} -s ${size}
    )
endfunction()

add_custom_test(VerySmallTests 100 10)
add_custom_test(SmallTests 100 100)
add_custom_test(NormalTests 100 1000)
add_custom_test(BigTests 100 10000)
add_custom_test(VeryBigTests 10 100000)
add_custom_test(HugeTests 10 1000000)
add_custom_test(FinalTests 100 1000000)

#=======================================================================
