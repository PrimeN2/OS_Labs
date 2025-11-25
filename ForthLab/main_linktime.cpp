#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include "contracts.h"

int main() {
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        auto pos = line.find(' ');
        std::string cmd = pos == std::string::npos ? line : line.substr(0, pos);
        std::vector<int> args;
        if (pos != std::string::npos) {
            std::string rest = line.substr(pos + 1);
            size_t start = 0;
            while ((pos = rest.find(' ', start)) != std::string::npos) {
                args.push_back(std::stoi(rest.substr(start, pos - start)));
                start = pos + 1;
            }
            args.push_back(std::stoi(rest.substr(start)));
        }

        if (cmd == "0") {
            std::cout << "Switching not supported in link-time version.\n";
        } else if (cmd == "1" && args.size() == 2) {
            int result = gcd(args[0], args[1]);
            std::cout << result << "\n";
        } else if (cmd == "2" && !args.empty()) {
            size_t n = args.size();
            int *sorted = sort(args.data(), n);
            for (size_t i = 0; i < n; ++i) {
                std::cout << sorted[i] << " ";
            }
            std::cout << "\n";
            free(sorted);
        } else {
            std::cout << "Invalid command or arguments.\n";
        }
    }
    return 0;
}
