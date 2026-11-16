#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>

using namespace std;

void print_buffers(const vector<char>& left, const vector<char>& right) {
    for (char c : left) {
        cout << c;
    }
    for (char c : right) {
        cout << c;
    }
    cout << endl;
}

int main() {
    int buf_Size;
    cout << "What is the Buffer size? (Minimum 4):" << endl;
    if (!(cin >> buf_Size) || buf_Size < 4) {
        cerr << "Error: Buffer size must be an integer >= 4." << endl;
        return 1;
    }

    // Clear trailing newline left in the input stream
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int half_size = static_cast<int>(ceil(buf_Size / 2.0));
    int usable_capacity = half_size - 1; // Last slot reserved for sentinel

    cout << "Enter the string:" << endl;
    string input;

    while (getline(cin, input)) {
        if (input.empty()) {
            continue;
        }

        // Initialize left and right buffers using standard vectors (replacing non-standard VLAs)
        vector<char> bufferLeft(half_size, ' ');
        vector<char> bufferRight(half_size, ' ');

        // Sentinels at the end of each buffer half
        bufferLeft[half_size - 1] = 'L';
        bufferRight[half_size - 1] = 'R';

        cout << "\n--- Buffer Fill Steps ---" << endl;
        for (size_t i = 0; i < input.length(); ++i) {
            int round = static_cast<int>(i / usable_capacity);
            int slot = static_cast<int>(i % usable_capacity);

            // Alternating dual-buffer scheme:
            // Even rounds fill the Left buffer, odd rounds fill the Right buffer
            if (round % 2 == 0) {
                // If starting a fresh Left buffer pass after previous cycles, clear old contents
                if (slot == 0 && i > 0) {
                    fill(bufferLeft.begin(), bufferLeft.end() - 1, ' ');
                }
                bufferLeft[slot] = input[i];
            } else {
                // If starting a fresh Right buffer pass after previous cycles, clear old contents
                if (slot == 0) {
                    fill(bufferRight.begin(), bufferRight.end() - 1, ' ');
                }
                bufferRight[slot] = input[i];
            }

            print_buffers(bufferLeft, bufferRight);
        }

        cout << "\nFinal Buffer State:" << endl;
        print_buffers(bufferLeft, bufferRight);
        cout << "\nEnter another string (or Ctrl+C to exit):" << endl;
    }

    return 0;
}
