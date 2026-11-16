# Lexical Annotator & Compiler Essentials (Pure C / C++)

This repository contains clean, robust implementations of foundational compiler front-end algorithms and mechanisms written in **pure C and C++**, along with a **native Windows GUI desktop application**.

---

## 🖥️ Native Desktop GUI Application (`gui.exe`)

You can run the full GUI application with tabs, text editors, and buttons without touching the terminal:

- **Source**: [`gui.cpp`](gui.cpp) (Pure C++ using Win32 API)
- **Run Directly**:
  ```powershell
  .\gui.exe
  ```
  *(Or simply double-click `gui.exe` in Windows Explorer)*
- **Features**:
  - **Tab 1 (Lexical Analyzer)**: Write or paste C code $\to$ click **"Analyze Code"** to see all tokens extracted.
  - **Tab 2 (Dual-Buffer Simulator)**: Input buffer size and string $\to$ click **"Simulate Dual-Buffer"** to trace step-by-step buffer fillings.
  - **Tab 3 (Left Recursion Eliminator)**: Input CFG production rules $\to$ click **"Eliminate Left Recursion"** to compute right-recursive forms.

---

## 💻 CLI Programs (Terminal)

### 1. Simple Lexical Analyzer (`lexer.c` $\to$ `lexer.exe`)
- **Compilation**: `cl lexer.c /Fe:lexer.exe`
- **Run**: `.\lexer.exe` (reads from `text1.txt`)

### 2. Dual-Buffer Input Scheme (`buffer_pairs.cpp` $\to$ `buffer_pairs.exe`)
- **Compilation**: `cl /std:c++17 /EHsc buffer_pairs.cpp /Fe:buffer_pairs.exe`
- **Run**: `.\buffer_pairs.exe`

### 3. Left Recursion Elimination (`left_recursion.cpp` $\to$ `left_recursion.exe`)
- **Compilation**: `cl /std:c++17 /EHsc left_recursion.cpp /Fe:left_recursion.exe`
- **Run**: `.\left_recursion.exe`

---

## Building with CMake
```bash
mkdir build
cd build
cmake ..
cmake --build .
```
