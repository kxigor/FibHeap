# Fibonacci Heap

![FibHeap](./images/art.png "Fibonacci Heap Graph")

## Требования
- СИ
- C++20 (g++ compiler)
- nasm
- graphviz
- cmake

### Компиляция тестового варианта
```bash
./runbuild.sh
```
### Запуск тестового варианта
```
./asm_fib_heap.out
```
Команды:
- 1 {number} (вставить узел)
- 2 (извлечь минимум)
- 3 (удалить последний добалвенный)

Куча будет отображаться в файле `fib_heap.png`
### Запуск тестов
```bash
./test.sh
```
### Эксплутатация
```bash
doxygen
```