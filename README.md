# Compiler Design Lab Assignments (CS 327)

This repository contains implementations of core compiler front-end algorithms and mechanisms.

---

## Programs

### 1. Simple Lexical Analyzer (`2.c`)
- **Purpose**: Reads a source text file (`text1.txt`) and tokenizes it into identifiers, numbers, and operators.
- **Compilation**:
  ```bash
  gcc -Wall -Wextra 2.c -o lexer
  ./lexer
  ```
- **Input**: Reads from `text1.txt` (a sample is provided).

### 2. Dual-Buffer Input Scheme (`assignment1.cpp`)
- **Purpose**: Simulates the Dragon Book two-buffer lookahead system (`bufferLeft` and `bufferRight`), each with a sentinel (`L` and `R`).
- **Compilation**:
  ```bash
  g++ -std=c++17 -Wall assignment1.cpp -o buffer_pairs
  ./buffer_pairs
  ```
- **Usage**:
  1. Enter a buffer size (e.g., `10`).
  2. Enter an input string (e.g., `E=M*C^2`).
  3. Observe how characters alternate between the left and right buffer partitions.

### 3. Left Recursion Elimination (`saidur.cpp` / `saidur.c`)
- **Purpose**: Eliminates direct and indirect left recursion from Context-Free Grammars (CFGs) for top-down predictive parsers.
- **Compilation**:
  ```bash
  g++ -std=c++17 -Wall saidur.cpp -o left_recursion
  ./left_recursion
  ```
- **Usage Example (Direct Left Recursion)**:
  - Productions count: `1`
  - Input: `A=Aa|b`
  - Result:
    - `A = b A'|`
    - `A' = a A'|^` (where `^` is epsilon)

---

## Building with CMake
```bash
mkdir build
cd build
cmake ..
cmake --build .
```
