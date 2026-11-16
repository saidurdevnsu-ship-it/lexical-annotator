import os
import sys
import subprocess
import tkinter as tk
from tkinter import ttk, messagebox, scrolledtext

DIR = os.path.dirname(os.path.abspath(__file__))
LEXER_EXE = os.path.join(DIR, "lexer.exe")
BUFFER_EXE = os.path.join(DIR, "buffer_pairs.exe")
RECURSION_EXE = os.path.join(DIR, "left_recursion.exe")
TEXT1 = os.path.join(DIR, "text1.txt")

class CompilerGUI(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Lexical Annotator & Compiler Essentials GUI")
        self.geometry("820x650")
        self.configure(bg="#0f172a")

        style = ttk.Style(self)
        style.theme_use("clam")
        style.configure("TNotebook", background="#0f172a", borderwidth=0)
        style.configure("TNotebook.Tab", background="#1e293b", foreground="#94a3b8", padding=[12, 6], font=("Segoe UI", 10, "bold"))
        style.map("TNotebook.Tab", background=[("selected", "#38bdf8")], foreground=[("selected", "#0f172a")])

        notebook = ttk.Notebook(self)
        notebook.pack(fill=tk.BOTH, expand=True, padx=12, pady=12)

        # Tab 1: Lexer
        tab_lex = tk.Frame(notebook, bg="#1e293b")
        notebook.add(tab_lex, text=" 1. Lexical Analyzer ")
        self.init_lexer_tab(tab_lex)

        # Tab 2: Buffer Pairs
        tab_buf = tk.Frame(notebook, bg="#1e293b")
        notebook.add(tab_buf, text=" 2. Dual-Buffer Simulator ")
        self.init_buffer_tab(tab_buf)

        # Tab 3: Left Recursion
        tab_rec = tk.Frame(notebook, bg="#1e293b")
        notebook.add(tab_rec, text=" 3. Left Recursion Eliminator ")
        self.init_recursion_tab(tab_rec)

    def init_lexer_tab(self, parent):
        tk.Label(parent, text="Source Code (C Language):", bg="#1e293b", fg="#f8fafc", font=("Segoe UI", 11, "bold")).pack(anchor=tk.W, padx=14, pady=(12, 4))
        self.lex_input = scrolledtext.ScrolledText(parent, height=9, bg="#0f172a", fg="#f8fafc", insertbackground="#fff", font=("Consolas", 10))
        self.lex_input.pack(fill=tk.BOTH, expand=False, padx=14, pady=4)
        self.lex_input.insert(tk.END, "int count = 100;\nfloat total_sum = count * 25 + 50;\nif (total_sum > 200) {\n    count = count - 1;\n}")

        btn_frame = tk.Frame(parent, bg="#1e293b")
        btn_frame.pack(anchor=tk.W, padx=14, pady=6)
        tk.Button(btn_frame, text="⚡ Analyze Code", bg="#38bdf8", fg="#0f172a", font=("Segoe UI", 10, "bold"), padx=12, pady=4, command=self.run_lexer).pack(side=tk.LEFT, padx=(0, 8))

        tk.Label(parent, text="Extracted Tokens:", bg="#1e293b", fg="#f8fafc", font=("Segoe UI", 11, "bold")).pack(anchor=tk.W, padx=14, pady=(8, 4))
        self.lex_output = scrolledtext.ScrolledText(parent, height=12, bg="#0f172a", fg="#a855f7", font=("Consolas", 10))
        self.lex_output.pack(fill=tk.BOTH, expand=True, padx=14, pady=(0, 12))

    def run_lexer(self):
        code = self.lex_input.get("1.0", tk.END).strip()
        with open(TEXT1, "w", encoding="utf-8") as f:
            f.write(code)
        proc = subprocess.run([LEXER_EXE], capture_output=True, text=True, cwd=DIR)
        self.lex_output.delete("1.0", tk.END)
        self.lex_output.insert(tk.END, proc.stdout)

    def init_buffer_tab(self, parent):
        form_frame = tk.Frame(parent, bg="#1e293b")
        form_frame.pack(fill=tk.X, padx=14, pady=12)

        tk.Label(form_frame, text="Buffer Size (>= 4):", bg="#1e293b", fg="#f8fafc", font=("Segoe UI", 10)).grid(row=0, column=0, sticky=tk.W, pady=4)
        self.buf_size_entry = tk.Entry(form_frame, font=("Segoe UI", 10), width=10, bg="#0f172a", fg="#f8fafc", insertbackground="#fff")
        self.buf_size_entry.grid(row=0, column=1, sticky=tk.W, padx=8, pady=4)
        self.buf_size_entry.insert(0, "10")

        tk.Label(form_frame, text="Input String:", bg="#1e293b", fg="#f8fafc", font=("Segoe UI", 10)).grid(row=1, column=0, sticky=tk.W, pady=4)
        self.buf_text_entry = tk.Entry(form_frame, font=("Segoe UI", 10), width=35, bg="#0f172a", fg="#f8fafc", insertbackground="#fff")
        self.buf_text_entry.grid(row=1, column=1, sticky=tk.W, padx=8, pady=4)
        self.buf_text_entry.insert(0, "E=M*C^2")

        tk.Button(form_frame, text="▶ Simulate Buffer", bg="#38bdf8", fg="#0f172a", font=("Segoe UI", 10, "bold"), padx=12, pady=4, command=self.run_buffer).grid(row=2, column=1, sticky=tk.W, padx=8, pady=8)

        tk.Label(parent, text="Buffer Transition Trace:", bg="#1e293b", fg="#f8fafc", font=("Segoe UI", 11, "bold")).pack(anchor=tk.W, padx=14, pady=(8, 4))
        self.buf_output = scrolledtext.ScrolledText(parent, height=15, bg="#0f172a", fg="#38bdf8", font=("Consolas", 10))
        self.buf_output.pack(fill=tk.BOTH, expand=True, padx=14, pady=(0, 12))

    def run_buffer(self):
        try:
            bsize = int(self.buf_size_entry.get().strip())
        except ValueError:
            messagebox.showerror("Error", "Buffer size must be a valid integer.")
            return
        text = self.buf_text_entry.get().strip()
        payload = f"{bsize}\n{text}\n"
        proc = subprocess.run([BUFFER_EXE], input=payload, capture_output=True, text=True, cwd=DIR)
        self.buf_output.delete("1.0", tk.END)
        self.buf_output.insert(tk.END, proc.stdout)

    def init_recursion_tab(self, parent):
        rec_frame = tk.Frame(parent, bg="#1e293b")
        rec_frame.pack(fill=tk.X, padx=14, pady=12)

        tk.Label(rec_frame, text="Production 1 (e.g. A=Aa|b):", bg="#1e293b", fg="#f8fafc", font=("Segoe UI", 10)).grid(row=0, column=0, sticky=tk.W, pady=4)
        self.rec_p1_entry = tk.Entry(rec_frame, font=("Segoe UI", 10), width=30, bg="#0f172a", fg="#f8fafc", insertbackground="#fff")
        self.rec_p1_entry.grid(row=0, column=1, sticky=tk.W, padx=8, pady=4)
        self.rec_p1_entry.insert(0, "A=Aa|b")

        tk.Button(rec_frame, text="✨ Eliminate Left Recursion", bg="#38bdf8", fg="#0f172a", font=("Segoe UI", 10, "bold"), padx=12, pady=4, command=self.run_recursion).grid(row=1, column=1, sticky=tk.W, padx=8, pady=8)

        tk.Label(parent, text="Resulting Grammar:", bg="#1e293b", fg="#f8fafc", font=("Segoe UI", 11, "bold")).pack(anchor=tk.W, padx=14, pady=(8, 4))
        self.rec_output = scrolledtext.ScrolledText(parent, height=15, bg="#0f172a", fg="#22c55e", font=("Consolas", 10))
        self.rec_output.pack(fill=tk.BOTH, expand=True, padx=14, pady=(0, 12))

    def run_recursion(self):
        p1 = self.rec_p1_entry.get().strip()
        payload = f"1\n{p1}\n"
        proc = subprocess.run([RECURSION_EXE], input=payload, capture_output=True, text=True, cwd=DIR)
        self.rec_output.delete("1.0", tk.END)
        self.rec_output.insert(tk.END, proc.stdout)

if __name__ == "__main__":
    app = CompilerGUI()
    app.mainloop()
