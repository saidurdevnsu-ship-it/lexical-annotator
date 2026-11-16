#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <cstring>
#include <cctype>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "comctl32.lib")

#define ID_TABCONTROL       1001
#define ID_BTN_RUN_LEXER    1002
#define ID_BTN_RUN_BUFFER   1003
#define ID_BTN_RUN_REC      1004

// Global control handles
HWND g_hTab = NULL;
HFONT g_hFont = NULL;

// Tab 1 Controls (Lexer)
HWND g_lblLexInput = NULL;
HWND g_txtLexInput = NULL;
HWND g_btnLexRun = NULL;
HWND g_lblLexOutput = NULL;
HWND g_txtLexOutput = NULL;

// Tab 2 Controls (Buffer Pairs)
HWND g_lblBufSize = NULL;
HWND g_txtBufSize = NULL;
HWND g_lblBufStr = NULL;
HWND g_txtBufStr = NULL;
HWND g_btnBufRun = NULL;
HWND g_lblBufOutput = NULL;
HWND g_txtBufOutput = NULL;

// Tab 3 Controls (Left Recursion)
HWND g_lblRecCount = NULL;
HWND g_txtRecCount = NULL;
HWND g_lblRecP1 = NULL;
HWND g_txtRecP1 = NULL;
HWND g_lblRecP2 = NULL;
HWND g_txtRecP2 = NULL;
HWND g_btnRecRun = NULL;
HWND g_lblRecOutput = NULL;
HWND g_txtRecOutput = NULL;

std::vector<HWND> g_tab1Controls;
std::vector<HWND> g_tab2Controls;
std::vector<HWND> g_tab3Controls;

// --- Core Algorithm Implementations in C++ ---

// 1. Lexical Analysis
std::string run_lexer_engine(const std::string& input) {
    std::ostringstream out;
    const char* ops = "+-*/%={}[]()><";
    size_t i = 0;
    size_t len = input.length();

    while (i < len) {
        char ch = input[i];

        // Identifiers: starts with alpha or underscore
        if (isalpha(static_cast<unsigned char>(ch)) || ch == '_') {
            std::string id;
            id += ch;
            i++;
            while (i < len && (isalnum(static_cast<unsigned char>(input[i])) || input[i] == '_')) {
                id += input[i++];
            }
            out << id << " is identifier\r\n";
            continue;
        }

        // Numbers: sequence of digits
        if (isdigit(static_cast<unsigned char>(ch))) {
            std::string num;
            num += ch;
            i++;
            while (i < len && isdigit(static_cast<unsigned char>(input[i]))) {
                num += input[i++];
            }
            out << num << " is number\r\n";
            continue;
        }

        // Operators
        if (strchr(ops, ch) != nullptr) {
            out << ch << " is operator\r\n";
            i++;
            continue;
        }

        // Whitespace and other characters are ignored delimiters
        i++;
    }
    return out.str();
}

// 2. Buffer Pairs Simulation
std::string run_buffer_engine(int buf_Size, const std::string& input) {
    if (buf_Size < 4) {
        return "Error: Buffer size must be at least 4.\r\n";
    }
    std::ostringstream out;
    int half_size = static_cast<int>(ceil(buf_Size / 2.0));
    int usable_capacity = half_size - 1;

    std::vector<char> bufferLeft(half_size, ' ');
    std::vector<char> bufferRight(half_size, ' ');
    bufferLeft[half_size - 1] = 'L';
    bufferRight[half_size - 1] = 'R';

    out << "--- Dual-Buffer Step-by-Step Simulation ---\r\n\r\n";

    for (size_t i = 0; i < input.length(); ++i) {
        int round = static_cast<int>(i / usable_capacity);
        int slot = static_cast<int>(i % usable_capacity);

        if (round % 2 == 0) {
            if (slot == 0 && i > 0) {
                std::fill(bufferLeft.begin(), bufferLeft.end() - 1, ' ');
            }
            bufferLeft[slot] = input[i];
        } else {
            if (slot == 0) {
                std::fill(bufferRight.begin(), bufferRight.end() - 1, ' ');
            }
            bufferRight[slot] = input[i];
        }

        for (char c : bufferLeft) out << c;
        for (char c : bufferRight) out << c;
        out << "\r\n";
    }

    out << "\r\nFinal Buffer State:\r\n";
    for (char c : bufferLeft) out << c;
    for (char c : bufferRight) out << c;
    out << "\r\n";

    return out.str();
}

// 3. Left Recursion Elimination
std::string eliminate_direct_recursion(const std::string& prod) {
    if (prod.length() < 3 || prod[1] != '=') {
        return "Invalid production format. Expected format: X=...\r\n";
    }
    char left = prod[0];
    int length = static_cast<int>(prod.length());
    std::ostringstream out;

    out << "Production: " << prod << "\r\n";
    out << "Non-Terminal = " << left << "\r\n\r\n";

    std::string alpha = "";
    std::string beta = "";
    bool flag1 = false;
    bool flag2 = false;

    int i = 2;
    while (i < length) {
        if (prod[i] == left) {
            int c = i + 1;
            std::string cur_alpha = "";
            while (c < length && prod[c] != '|' && prod[c] != '\0') {
                cur_alpha += prod[c++];
            }
            alpha += cur_alpha + "|";
            flag1 = true;
            i = (c < length && prod[c] == '|') ? c + 1 : c;
        } else {
            int z = i;
            std::string cur_beta = "";
            while (z < length && prod[z] != '|' && prod[z] != '\0') {
                cur_beta += prod[z++];
            }
            beta += cur_beta + "|";
            flag2 = true;
            i = (z < length && prod[z] == '|') ? z + 1 : z;
        }
    }

    if (!flag1) {
        out << "No direct left recursion found in: " << prod << "\r\n";
        return out.str();
    }

    out << "alpha (recursive parts)     : " << alpha << "\r\n";
    out << "beta  (non-recursive parts) : " << beta << "\r\n\r\n";
    out << "--- Resulting Grammar (Left Recursion Removed) ---\r\n\r\n";

    // X = beta1 X' | beta2 X' ...
    out << left << " = ";
    for (size_t b = 0; b < beta.length(); ++b) {
        if (beta[b] != '|') {
            out << beta[b];
        } else {
            out << " " << left << "'|";
        }
    }
    out << "\r\n";

    // X' = alpha1 X' | ... | ^
    out << left << "' = ";
    size_t a = 0;
    while (a < alpha.length()) {
        while (a < alpha.length() && alpha[a] != '|') {
            out << alpha[a++];
        }
        if (a < alpha.length() && alpha[a] == '|') {
            out << " " << left << "'|";
            a++;
        }
    }
    out << "^\r\n"; // '^' denotes epsilon

    return out.str();
}

std::string run_left_recursion_engine(int count, const std::string& p1, const std::string& p2) {
    if (count <= 1) {
        return eliminate_direct_recursion(p1);
    }
    // 2 productions substitution
    if (p1.length() < 3 || p2.length() < 3) {
        return "Invalid productions format.\r\n";
    }
    char left1 = p1[0];
    char left2 = p2[0];
    std::string substituted = "";
    substituted += left1;
    substituted += '=';

    size_t len1 = p1.length();
    size_t len2 = p2.length();

    for (size_t i = 2; i < len1; ++i) {
        if (p1[i] == left2) {
            size_t h = i + 1;
            std::string temp1 = "";
            while (h < len1 && p1[h] != '|' && p1[h] != '\0') {
                temp1 += p1[h++];
            }

            size_t p = 2;
            while (p < len2) {
                while (p < len2 && p2[p] != '|' && p2[p] != '\0') {
                    substituted += p2[p++];
                }
                substituted += temp1;
                substituted += '|';
                if (p < len2 && p2[p] == '|') p++;
            }
            i = h;
        } else {
            substituted += p1[i];
        }
    }

    std::ostringstream out;
    out << "Substituted Production: " << substituted << "\r\n\r\n";
    out << eliminate_direct_recursion(substituted);
    return out.str();
}

void show_tab(int tabIndex) {
    for (HWND h : g_tab1Controls) ShowWindow(h, tabIndex == 0 ? SW_SHOW : SW_HIDE);
    for (HWND h : g_tab2Controls) ShowWindow(h, tabIndex == 1 ? SW_SHOW : SW_HIDE);
    for (HWND h : g_tab3Controls) ShowWindow(h, tabIndex == 2 ? SW_SHOW : SW_HIDE);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        INITCOMMONCONTROLSEX icex;
        icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
        icex.dwICC = ICC_TAB_CLASSES;
        InitCommonControlsEx(&icex);

        g_hFont = CreateFontW(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");

        // Tab Control
        g_hTab = CreateWindowW(WC_TABCONTROLW, L"", WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,
            15, 15, 755, 595, hWnd, (HMENU)ID_TABCONTROL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
        SendMessage(g_hTab, WM_SETFONT, (WPARAM)g_hFont, TRUE);

        TCITEMW tie;
        tie.mask = TCIF_TEXT;
        tie.pszText = (LPWSTR)L"1. Lexical Analyzer";
        TabCtrl_InsertItem(g_hTab, 0, &tie);
        tie.pszText = (LPWSTR)L"2. Dual-Buffer Simulator";
        TabCtrl_InsertItem(g_hTab, 1, &tie);
        tie.pszText = (LPWSTR)L"3. Left Recursion Eliminator";
        TabCtrl_InsertItem(g_hTab, 2, &tie);

        // --- TAB 1: LEXER ---
        g_lblLexInput = CreateWindowW(L"STATIC", L"Source Code (C Language):", WS_CHILD | SS_LEFT,
            35, 60, 300, 20, hWnd, NULL, NULL, NULL);
        g_txtLexInput = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT",
            L"int count = 100;\r\nfloat total_sum = count * 25 + 50;\r\nif (total_sum > 200) {\r\n    count = count - 1;\r\n}",
            WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL,
            35, 85, 715, 130, hWnd, NULL, NULL, NULL);

        g_btnLexRun = CreateWindowW(L"BUTTON", L"Analyze Code", WS_CHILD | BS_DEFPUSHBUTTON,
            35, 225, 140, 32, hWnd, (HMENU)ID_BTN_RUN_LEXER, NULL, NULL);

        g_lblLexOutput = CreateWindowW(L"STATIC", L"Extracted Tokens Output:", WS_CHILD | SS_LEFT,
            35, 268, 300, 20, hWnd, NULL, NULL, NULL);
        g_txtLexOutput = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY | WS_VSCROLL,
            35, 290, 715, 300, hWnd, NULL, NULL, NULL);

        g_tab1Controls = { g_lblLexInput, g_txtLexInput, g_btnLexRun, g_lblLexOutput, g_txtLexOutput };

        // --- TAB 2: BUFFER ---
        g_lblBufSize = CreateWindowW(L"STATIC", L"Buffer Size (>= 4):", WS_CHILD | SS_LEFT,
            35, 65, 150, 20, hWnd, NULL, NULL, NULL);
        g_txtBufSize = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"10",
            WS_CHILD | ES_AUTOHSCROLL,
            190, 62, 80, 24, hWnd, NULL, NULL, NULL);

        g_lblBufStr = CreateWindowW(L"STATIC", L"Input String:", WS_CHILD | SS_LEFT,
            35, 100, 150, 20, hWnd, NULL, NULL, NULL);
        g_txtBufStr = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"E=M*C^2",
            WS_CHILD | ES_AUTOHSCROLL,
            190, 97, 280, 24, hWnd, NULL, NULL, NULL);

        g_btnBufRun = CreateWindowW(L"BUTTON", L"Simulate Dual-Buffer", WS_CHILD | BS_DEFPUSHBUTTON,
            190, 132, 160, 32, hWnd, (HMENU)ID_BTN_RUN_BUFFER, NULL, NULL);

        g_lblBufOutput = CreateWindowW(L"STATIC", L"Buffer Fill Steps & Final State:", WS_CHILD | SS_LEFT,
            35, 175, 300, 20, hWnd, NULL, NULL, NULL);
        g_txtBufOutput = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY | WS_VSCROLL,
            35, 198, 715, 390, hWnd, NULL, NULL, NULL);

        g_tab2Controls = { g_lblBufSize, g_txtBufSize, g_lblBufStr, g_txtBufStr, g_btnBufRun, g_lblBufOutput, g_txtBufOutput };

        // --- TAB 3: LEFT RECURSION ---
        g_lblRecCount = CreateWindowW(L"STATIC", L"Productions Count (1 or 2):", WS_CHILD | SS_LEFT,
            35, 65, 180, 20, hWnd, NULL, NULL, NULL);
        g_txtRecCount = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"1",
            WS_CHILD | ES_AUTOHSCROLL,
            220, 62, 60, 24, hWnd, NULL, NULL, NULL);

        g_lblRecP1 = CreateWindowW(L"STATIC", L"Production 1 (e.g. A=Aa|b):", WS_CHILD | SS_LEFT,
            35, 100, 180, 20, hWnd, NULL, NULL, NULL);
        g_txtRecP1 = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"A=Aa|b",
            WS_CHILD | ES_AUTOHSCROLL,
            220, 97, 280, 24, hWnd, NULL, NULL, NULL);

        g_lblRecP2 = CreateWindowW(L"STATIC", L"Production 2 (optional):", WS_CHILD | SS_LEFT,
            35, 135, 180, 20, hWnd, NULL, NULL, NULL);
        g_txtRecP2 = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"A=Sc|d",
            WS_CHILD | ES_AUTOHSCROLL,
            220, 132, 280, 24, hWnd, NULL, NULL, NULL);

        g_btnRecRun = CreateWindowW(L"BUTTON", L"Eliminate Left Recursion", WS_CHILD | BS_DEFPUSHBUTTON,
            220, 168, 180, 32, hWnd, (HMENU)ID_BTN_RUN_REC, NULL, NULL);

        g_lblRecOutput = CreateWindowW(L"STATIC", L"Resulting Grammar Output:", WS_CHILD | SS_LEFT,
            35, 210, 300, 20, hWnd, NULL, NULL, NULL);
        g_txtRecOutput = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY | WS_VSCROLL,
            35, 233, 715, 355, hWnd, NULL, NULL, NULL);

        g_tab3Controls = { g_lblRecCount, g_txtRecCount, g_lblRecP1, g_txtRecP1, g_lblRecP2, g_txtRecP2, g_btnRecRun, g_lblRecOutput, g_txtRecOutput };

        // Apply font to all controls
        auto apply_font = [](const std::vector<HWND>& ctrls) {
            for (HWND h : ctrls) SendMessage(h, WM_SETFONT, (WPARAM)g_hFont, TRUE);
        };
        apply_font(g_tab1Controls);
        apply_font(g_tab2Controls);
        apply_font(g_tab3Controls);

        show_tab(0);
        break;
    }
    case WM_NOTIFY: {
        LPNMHDR lpnmhdr = (LPNMHDR)lParam;
        if (lpnmhdr->idFrom == ID_TABCONTROL && lpnmhdr->code == TCN_SELCHANGE) {
            int selectedTab = TabCtrl_GetCurSel(g_hTab);
            show_tab(selectedTab);
        }
        break;
    }
    case WM_COMMAND: {
        int id = LOWORD(wParam);
        if (id == ID_BTN_RUN_LEXER) {
            int len = GetWindowTextLengthA(g_txtLexInput);
            std::string code(len + 1, '\0');
            GetWindowTextA(g_txtLexInput, &code[0], len + 1);
            code.resize(len);

            std::string result = run_lexer_engine(code);
            SetWindowTextA(g_txtLexOutput, result.c_str());
        }
        else if (id == ID_BTN_RUN_BUFFER) {
            char sizeBuf[16] = {0};
            GetWindowTextA(g_txtBufSize, sizeBuf, 15);
            int bufSize = atoi(sizeBuf);

            int len = GetWindowTextLengthA(g_txtBufStr);
            std::string str(len + 1, '\0');
            GetWindowTextA(g_txtBufStr, &str[0], len + 1);
            str.resize(len);

            std::string result = run_buffer_engine(bufSize, str);
            SetWindowTextA(g_txtBufOutput, result.c_str());
        }
        else if (id == ID_BTN_RUN_REC) {
            char countBuf[16] = {0};
            GetWindowTextA(g_txtRecCount, countBuf, 15);
            int count = atoi(countBuf);

            int len1 = GetWindowTextLengthA(g_txtRecP1);
            std::string p1(len1 + 1, '\0');
            GetWindowTextA(g_txtRecP1, &p1[0], len1 + 1);
            p1.resize(len1);

            int len2 = GetWindowTextLengthA(g_txtRecP2);
            std::string p2(len2 + 1, '\0');
            GetWindowTextA(g_txtRecP2, &p2[0], len2 + 1);
            p2.resize(len2);

            std::string result = run_left_recursion_engine(count, p1, p2);
            SetWindowTextA(g_txtRecOutput, result.c_str());
        }
        break;
    }
    case WM_DESTROY:
        if (g_hFont) DeleteObject(g_hFont);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcW(hWnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)hPrevInstance; (void)lpCmdLine;
    const wchar_t CLASS_NAME[] = L"CompilerEssentialsGUIClass";

    WNDCLASSEXW wc = {0};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);

    if (!RegisterClassExW(&wc)) {
        return 0;
    }

    HWND hWnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Lexical Annotator & Compiler Essentials GUI (Pure C++)",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 660,
        NULL, NULL, hInstance, NULL
    );

    if (!hWnd) {
        return 0;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
