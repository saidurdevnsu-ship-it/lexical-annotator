# Lexical Annotator & Compiler Essentials (CS 327)

This repository contains clean, robust implementations of foundational compiler front-end algorithms and mechanisms.

---

## Source Files & Programs

### 1. Simple Lexical Analyzer (`lexer.c`)
- **Purpose**: Reads a source text file (`text1.txt`) and tokenizes it into identifiers, numbers, and operators using standard finite-state lexical analysis.
- **Compilation**:
  ```bash
  gcc -Wall -Wextra lexer.c -o lexer
  ./lexer
  ```
  *(Or on MSVC: `cl lexer.c /Fe:lexer.exe`)*
- **Input**: Reads from `text1.txt` (a sample is provided).

### 2. Dual-Buffer Input Scheme (`buffer_pairs.cpp`)
- **Purpose**: Simulates the Dragon Book two-buffer lookahead system (`bufferLeft` and `bufferRight`), each with a sentinel marker (`L` and `R`).
- **Compilation**:
  ```bash
  g++ -std=c++17 -Wall buffer_pairs.cpp -o buffer_pairs
  ./buffer_pairs
  ```
  *(Or on MSVC: `cl /std:c++17 /EHsc buffer_pairs.cpp /Fe:buffer_pairs.exe`)*
- **Usage**:
  1. Enter a buffer size (e.g., `10`).
  2. Enter an input string (e.g., `E=M*C^2`).
  3. Observe how characters alternate between the left and right buffer partitions.

### 3. Left Recursion Elimination (`left_recursion.cpp`)
- **Purpose**: Eliminates direct and indirect left recursion from Context-Free Grammars (CFGs) for top-down predictive parsers.
- **Compilation**:
  ```bash
  g++ -std=c++17 -Wall left_recursion.cpp -o left_recursion
  ./left_recursion
  ```
  *(Or on MSVC: `cl /std:c++17 /EHsc left_recursion.cpp /Fe:left_recursion.exe`)*
- **Usage Example (Direct Left Recursion)**:
  - Productions count: `1`
  - Input: `A=Aa|b`
  - Result:
    - `A = b A'|`
    - `A' = a A'|^` (where `^` is epsilon)

### 4. Automated Test Suite (`test_suite.py`)
- Run all edge case and regression tests with:
  ```bash
  python test_suite.py
  ```

---

## Building with CMake
```bash
mkdir build
cd build
cmake ..
cmake --build .
```
