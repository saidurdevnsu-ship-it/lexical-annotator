import os
import subprocess
import sys

DIR = os.path.dirname(os.path.abspath(__file__))
LEXER_EXE = os.path.join(DIR, "lexer.exe")
BUFFER_EXE = os.path.join(DIR, "buffer_pairs.exe")
RECURSION_EXE = os.path.join(DIR, "left_recursion.exe")
TEXT1 = os.path.join(DIR, "text1.txt")

def test_lexer():
    print("=== Testing Lexical Analyzer (lexer.exe) ===")
    test_cases = [
        (
            "int a = 123 + 456;",
            [
                "int is identifier",
                "a is identifier",
                "= is operator",
                "123 is number",
                "+ is operator",
                "456 is number"
            ]
        ),
        (
            "var_1=count2*3-x/4%5",
            [
                "var_1 is identifier",
                "= is operator",
                "count2 is identifier",
                "* is operator",
                "3 is number",
                "- is operator",
                "x is identifier",
                "/ is operator",
                "4 is number",
                "% is operator",
                "5 is number"
            ]
        ),
        (
            "if(x>10 && y<20){return a[0];}",
            [
                "if is identifier",
                "( is operator",
                "x is identifier",
                "> is operator",
                "10 is number",
                "y is identifier",
                "< is operator",
                "20 is number",
                ") is operator",
                "{ is operator",
                "return is identifier",
                "a is identifier",
                "[ is operator",
                "0 is number",
                "] is operator",
                "} is operator"
            ]
        ),
        (
            "", # empty file
            []
        ),
        (
            "   \n\t   ", # only whitespace
            []
        ),
        (
            "ident_at_eof", # no trailing newline
            ["ident_at_eof is identifier"]
        ),
        (
            "99999", # number at eof with no trailing newline
            ["99999 is number"]
        )
    ]

    for idx, (content, expected_tokens) in enumerate(test_cases, 1):
        with open(TEXT1, "w", encoding="utf-8") as f:
            f.write(content)
        
        proc = subprocess.run([LEXER_EXE], capture_output=True, text=True, cwd=DIR)
        assert proc.returncode == 0, f"Lexer failed with return code {proc.returncode} on case {idx}"
        
        actual_output = [line.strip() for line in proc.stdout.strip().splitlines() if line.strip()]
        for expected in expected_tokens:
            assert expected in actual_output, f"Case {idx} FAIL: Expected '{expected}' in output, got: {actual_output}"
        
        if not expected_tokens:
            assert len(actual_output) == 0, f"Case {idx} FAIL: Expected empty output for whitespace/empty input, got {actual_output}"

        print(f"  [PASS] Lexer Test Case {idx}: '{content[:30]}...' -> {len(expected_tokens)} tokens verified")

    print("[ALL PASS] Lexer passed all edge case tests!\n")


def test_buffer_pairs():
    print("=== Testing Buffer Pairs (buffer_pairs.exe) ===")
    test_cases = [
        ("10\nABC\n", ["ABC L    R", "Final Buffer State:\nABC L    R"]),
        ("8\n1234567890\n", ["Final Buffer State:"]),
        ("6\nHELLO_WORLD\n", ["Final Buffer State:"])
    ]

    for idx, (input_data, expected_snippets) in enumerate(test_cases, 1):
        proc = subprocess.run([BUFFER_EXE], input=input_data, capture_output=True, text=True, cwd=DIR)
        assert proc.returncode == 0, f"Buffer test failed with return code {proc.returncode} on case {idx}"
        out = proc.stdout
        for snippet in expected_snippets:
            assert snippet in out, f"Case {idx} FAIL: Snippet '{snippet}' not found in output:\n{out}"
        print(f"  [PASS] Buffer Pairs Test Case {idx} verified")

    # Invalid input test (< 4)
    proc = subprocess.run([BUFFER_EXE], input="2\n", capture_output=True, text=True, cwd=DIR)
    assert proc.returncode != 0, "Buffer size < 4 should fail but succeeded!"
    print("  [PASS] Buffer Pairs Reject Invalid Size (< 4) verified")

    print("[ALL PASS] Buffer Pairs passed all tests!\n")


def test_left_recursion():
    print("=== Testing Left Recursion Elimination (left_recursion.exe) ===")
    test_cases = [
        # Direct left recursion
        ("1\nA=Aa|b\n", ["A = b A'|", "A' = a A'|^"]),
        ("1\nE=E+T|T\n", ["E = T E'|", "E' = +T E'|^"]),
        ("1\nT=T*F|F\n", ["T = F T'|", "T' = *F T'|^"]),
        # No left recursion
        ("1\nS=a|b\n", ["No direct left recursion found"]),
        # 2 productions with indirect substitution
        ("2\nS=Aa|b\nA=Sc|d\n", ["Substituted Production:", "S' = "])
    ]

    for idx, (input_data, expected_snippets) in enumerate(test_cases, 1):
        proc = subprocess.run([RECURSION_EXE], input=input_data, capture_output=True, text=True, cwd=DIR)
        assert proc.returncode == 0, f"Left Recursion test failed with return code {proc.returncode} on case {idx}"
        out = proc.stdout
        for snippet in expected_snippets:
            assert snippet in out, f"Case {idx} FAIL: Snippet '{snippet}' not found in output:\n{out}"
        print(f"  [PASS] Left Recursion Test Case {idx} verified")

    print("[ALL PASS] Left Recursion passed all tests!\n")


if __name__ == "__main__":
    try:
        test_lexer()
        test_buffer_pairs()
        test_left_recursion()
        # Restore default text1.txt
        default_content = (
            "int count = 100;\n"
            "float total_sum = count * 25 + 50;\n"
            "if (total_sum > 200) {\n"
            "    count = count - 1;\n"
            "}\n"
        )
        with open(TEXT1, "w", encoding="utf-8") as f:
            f.write(default_content)
        print("*******************************************")
        print("  ALL AGGRESSIVE TESTS PASSED SUCCESSFULLY! ")
        print("*******************************************")
    except AssertionError as e:
        print(f"\n[TEST SUITE FAILURE] {e}")
        sys.exit(1)
