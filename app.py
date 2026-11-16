import os
import sys
import json
import math
import subprocess
import webbrowser
from http.server import HTTPServer, BaseHTTPRequestHandler

DIR = os.path.dirname(os.path.abspath(__file__))
WEB_DIR = os.path.join(DIR, "web")
LEXER_EXE = os.path.join(DIR, "lexer.exe")
BUFFER_EXE = os.path.join(DIR, "buffer_pairs.exe")
RECURSION_EXE = os.path.join(DIR, "left_recursion.exe")
TEXT1 = os.path.join(DIR, "text1.txt")

PORT = 5050

class AppHandler(BaseHTTPRequestHandler):
    def _send_json(self, data, status=200):
        body = json.dumps(data).encode("utf-8")
        self.send_response(status)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Access-Control-Allow-Origin", "*")
        self.end_headers()
        self.wfile.write(body)

    def do_GET(self):
        if self.path in ("/", "/index.html"):
            index_path = os.path.join(WEB_DIR, "index.html")
            if os.path.exists(index_path):
                with open(index_path, "rb") as f:
                    content = f.read()
                self.send_response(200)
                self.send_header("Content-Type", "text/html; charset=utf-8")
                self.send_header("Content-Length", str(len(content)))
                self.end_headers()
                self.wfile.write(content)
                return
        self.send_error(404, "File Not Found")

    def do_POST(self):
        length = int(self.headers.get("Content-Length", 0))
        post_data = self.rfile.read(length).decode("utf-8") if length > 0 else "{}"
        try:
            req = json.loads(post_data)
        except Exception:
            req = {}

        if self.path == "/api/lex":
            code = req.get("code", "")
            with open(TEXT1, "w", encoding="utf-8") as f:
                f.write(code)

            proc = subprocess.run([LEXER_EXE], capture_output=True, text=True, cwd=DIR)
            tokens = []
            counts = {"identifier": 0, "number": 0, "operator": 0}
            
            for line in proc.stdout.splitlines():
                line = line.strip()
                if " is identifier" in line:
                    val = line.replace(" is identifier", "").strip()
                    tokens.append({"value": val, "type": "identifier"})
                    counts["identifier"] += 1
                elif " is number" in line:
                    val = line.replace(" is number", "").strip()
                    tokens.append({"value": val, "type": "number"})
                    counts["number"] += 1
                elif " is operator" in line:
                    val = line.replace(" is operator", "").strip()
                    tokens.append({"value": val, "type": "operator"})
                    counts["operator"] += 1

            self._send_json({"tokens": tokens, "counts": counts, "raw": proc.stdout})

        elif self.path == "/api/buffer":
            buf_size = int(req.get("buffer_size", 10))
            text = req.get("text", "")
            input_payload = f"{buf_size}\n{text}\n"

            proc = subprocess.run([BUFFER_EXE], input=input_payload, capture_output=True, text=True, cwd=DIR)
            out = proc.stdout
            half_size = math.ceil(buf_size / 2)
            final_left = ""
            final_right = ""

            if "Final Buffer State:" in out:
                state_line = out.split("Final Buffer State:")[1].strip().splitlines()[0]
                if len(state_line) >= half_size * 2:
                    final_left = state_line[:half_size]
                    final_right = state_line[half_size:half_size*2]

            self._send_json({
                "raw": out,
                "half_size": half_size,
                "final_left": final_left,
                "final_right": final_right
            })

        elif self.path == "/api/left-recursion":
            count = int(req.get("count", 1))
            p1 = req.get("p1", "").strip()
            p2 = req.get("p2", "").strip()

            if count == 1:
                input_payload = f"1\n{p1}\n"
            else:
                input_payload = f"2\n{p1}\n{p2}\n"

            proc = subprocess.run([RECURSION_EXE], input=input_payload, capture_output=True, text=True, cwd=DIR)
            out = proc.stdout

            is_recursive = "Resulting Grammar (Left Recursion Removed)" in out
            alpha = ""
            beta = ""
            rule_base = ""
            rule_prime = ""

            for line in out.splitlines():
                if "alpha (recursive parts)" in line:
                    alpha = line.split(":", 1)[1].strip()
                elif "beta  (non-recursive parts)" in line:
                    beta = line.split(":", 1)[1].strip()
                elif " = " in line and not line.startswith("Non-Terminal") and not line.startswith("Production"):
                    if "'" in line.split("=")[0]:
                        rule_prime = line.strip()
                    else:
                        rule_base = line.strip()

            self._send_json({
                "is_recursive": is_recursive,
                "alpha": alpha,
                "beta": beta,
                "rule_base": rule_base,
                "rule_prime": rule_prime,
                "raw": out
            })
        else:
            self.send_error(404, "Endpoint not found")

def main():
    server_address = ("127.0.0.1", PORT)
    httpd = HTTPServer(server_address, AppHandler)
    url = f"http://localhost:{PORT}"
    print(f"==================================================")
    print(f"  Lexical Annotator Web GUI is running at:")
    print(f"  👉  {url}")
    print(f"==================================================")
    webbrowser.open(url)
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print("\nShutting down server.")
        httpd.server_close()

if __name__ == "__main__":
    main()
