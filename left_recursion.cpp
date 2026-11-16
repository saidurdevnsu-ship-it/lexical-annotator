#include <iostream>
#include <string>
#include <cstring>

using namespace std;

#define MAX_LEN 512

class LeftRecursionEliminator {
public:
    char S[MAX_LEN], A[MAX_LEN];
    char left1, left2;

    bool flag1 = false; // Flag for alpha (left recursion detected)
    bool flag2 = false; // Flag for beta (non-left-recursive terms)
    bool flag3 = false;

    char beta[MAX_LEN], alpha[MAX_LEN];
    char temp1[MAX_LEN], temp2[MAX_LEN];

    int j = 0, k = 0;
    int q = 0, o = 0;

    void direct(char production[]) {
        int length = static_cast<int>(strlen(production));
        cout << "\nProduction Length = " << length;
        char left = production[0];
        cout << "\nNon-Terminal = " << left << "\n";

        j = 0;
        k = 0;
        flag1 = false;
        flag2 = false;
        alpha[0] = '\0';
        beta[0] = '\0';

        // Grammar format expected: X=...
        // Start scanning alternatives after "X="
        int i = 2;
        while (i < length) {
            if (production[i] == left) {
                // Direct left recursion detected: X -> X alpha
                int c = i + 1;
                while (c < length && production[c] != '|' && production[c] != '\0') {
                    alpha[j++] = production[c++];
                }
                alpha[j++] = '|';
                alpha[j] = '\0';
                flag1 = true;
                i = (c < length && production[c] == '|') ? c + 1 : c;
            } else {
                // Beta term (no left recursion): X -> beta
                int z = i;
                while (z < length && production[z] != '|' && production[z] != '\0') {
                    beta[k++] = production[z++];
                }
                beta[k++] = '|';
                beta[k] = '\0';
                flag2 = true;
                i = (z < length && production[z] == '|') ? z + 1 : z;
            }
        }

        cout << "---------------------------------\n";
        if (flag1) {
            cout << "alpha (recursive parts)     : ";
            for (i = 0; alpha[i] != '\0'; i++) cout << alpha[i] << " ";
            cout << "\n";
        }
        if (flag2) {
            cout << "beta  (non-recursive parts) : ";
            for (i = 0; beta[i] != '\0'; i++) cout << beta[i] << " ";
            cout << "\n";
        }

        if (!flag1) {
            cout << "\nNo direct left recursion found in: " << production << "\n";
            return;
        }

        cout << "\n--- Resulting Grammar (Left Recursion Removed) ---\n";
        
        // Output: X = beta1 X' | beta2 X' ...
        cout << left << " = ";
        for (i = 0; beta[i] != '\0'; i++) {
            if (beta[i] != '|') {
                cout << beta[i];
            } else {
                cout << " " << left << "'|";
            }
        }
        cout << "\n";

        // Output: X' = alpha1 X' | alpha2 X' ... | ^ (where ^ represents epsilon)
        cout << left << "' = ";
        for (i = 0; alpha[i] != '\0'; i++) {
            while (alpha[i] != '\0' && alpha[i] != '|') {
                cout << alpha[i];
                i++;
            }
            if (alpha[i] == '|') {
                cout << " " << left << "'|";
            }
        }
        cout << "^\n"; // '^' denotes epsilon (null production)
    }
} r;

int main() {
    cout << "Enter the number of productions (1 or 2): ";
    int count;
    if (!(cin >> count)) {
        cerr << "Invalid input." << endl;
        return 1;
    }

    if (count > 1) {
        cout << "Enter production 1 (e.g. S=Aa|b):\n";
        cin >> r.S;
        cout << "Enter production 2 (e.g. A=Ac|Sd):\n";
        cin >> r.A;

        cout << "---------------------------------\n";
        cout << "Entered Production 1: " << r.S << "\n";
        cout << "Entered Production 2: " << r.A << "\n";

        int len1 = static_cast<int>(strlen(r.S));
        int len2 = static_cast<int>(strlen(r.A));

        r.S[len1] = '\0';
        r.A[len2] = '\0';
        r.left1 = r.S[0];
        r.left2 = r.A[0];

        cout << "left1 = " << r.left1 << ", left2 = " << r.left2 << "\n";
        cout << "---------------------------------\n";

        // Construct substituted production: substitute A into S if S has A
        r.temp2[r.q++] = r.left1;
        r.temp2[r.q++] = '=';

        for (int i = 2; i < len1; i++) {
            if (r.S[i] == r.left2) {
                r.flag3 = true;
                int h = i + 1;
                r.o = 0;

                while (h < len1 && r.S[h] != '|' && r.S[h] != '\0') {
                    r.temp1[r.o++] = r.S[h++];
                }
                r.temp1[r.o] = '\0';

                int p = 2;
                while (p < len2) {
                    while (p < len2 && r.A[p] != '|' && r.A[p] != '\0') {
                        r.temp2[r.q++] = r.A[p++];
                    }
                    for (int g = 0; g < (int)strlen(r.temp1); g++) {
                        r.temp2[r.q++] = r.temp1[g];
                    }
                    r.temp2[r.q++] = '|';
                    if (p < len2 && r.A[p] == '|') {
                        p++;
                    }
                }
                i = h;
            } else {
                r.temp2[r.q++] = r.S[i];
            }
        }
        r.temp2[r.q] = '\0';

        cout << "\nSubstituted Production: " << r.temp2 << "\n";
        r.direct(r.temp2);
    } else {
        cout << "Enter production (e.g. A=Aa|b):\n";
        cin >> r.temp2;
        cout << "---------------------------------\n";
        cout << "Entered Production: " << r.temp2 << "\n";
        r.direct(r.temp2);
    }

    return 0;
}
