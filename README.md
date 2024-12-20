# Fibonacci Heap

![FibHeap](./images/art.png "Fibonacci Heap Graph")

## Requirements
- C
- C++20 (g++ compiler)
- NASM
- Graphviz
- CMake

## Requirements for the heap only
- C or NASM

### Compiling the test case
```bash
./runbuild.sh
```
### Running the test case
```
./asm_fib_heap.out
```
Commands:
- 1 {number} (insert node)
- 2 (extract minimum)
- 3 (delete the last inserted)

The heap will be displayed in the file `fib_heap.png`

#### Example
```bash
1 1
1 2
1 3
1 4
1 5
1 6
1 5
1 4
1 3
1 2
1 1
3
2
3
```
Result:
![Result](./images/example.png "I love fibheap")

### Running tests
```bash
./test.sh
```
### Documentation
```bash
doxygen
```