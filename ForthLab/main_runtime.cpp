#include <iostream>
#include <string>
#include <vector>
#include <dlfcn.h>
#include <thread>
#include <mutex>
#include <cstdlib>

using GcdFunc = int(*)(int, int);
using SortFunc = int*(*)(int*, size_t);

std::mutex print_mutex;
GcdFunc gcd_ptr = nullptr;
SortFunc sort_ptr = nullptr;
void *gcd_handle = nullptr;
void *sort_handle = nullptr;

#ifdef __APPLE__
    const char *gcd_libs[2] = {"./lib/libgcd_euclid.dylib", "./lib/libgcd_naive.dylib"};
    const char *sort_libs[2] = {"./lib/libsort_bubble.dylib", "./lib/libsort_quicksort.dylib"};
#else
    const char *gcd_libs[2] = {"./lib/libgcd_euclid.so", "./lib/libgcd_naive.so"};
    const char *sort_libs[2] = {"./lib/libsort_bubble.so", "./lib/libsort_quicksort.so"};
#endif

int current_impl = 0;

bool load_libraries(int index) {
    dlerror();

    if (gcd_handle) dlclose(gcd_handle);
    if (sort_handle) dlclose(sort_handle);

    gcd_handle = dlopen(gcd_libs[index], RTLD_LAZY);
    sort_handle = dlopen(sort_libs[index], RTLD_LAZY);

    const char *error;
    if ((error = dlerror()) != nullptr) {
        std::lock_guard<std::mutex> lock(print_mutex);
        std::cerr << "Load error: " << error << "\n";
        return false;
    }

    gcd_ptr = (GcdFunc)dlsym(gcd_handle, "gcd");
    sort_ptr = (SortFunc)dlsym(sort_handle, "sort");

    if ((error = dlerror()) != nullptr) {
        std::lock_guard<std::mutex> lock(print_mutex);
        std::cerr << "Symbol error: " << error << "\n";
        return false;
    }

    return true;
}

void print_array(const int *arr, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
}

int main() {
    if (!load_libraries(current_impl)) {
        return 1;
    }

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
            current_impl = 1 - current_impl;
            if (load_libraries(current_impl)) {
                std::cout << "Switched to implementation " << (current_impl + 1) << "\n";
            } else {
                std::cout << "Failed to switch.\n";
                current_impl = 1 - current_impl;
                load_libraries(current_impl);
            }
        } else if (cmd == "1" && args.size() == 2) {
            int result = gcd_ptr(args[0], args[1]);
            std::cout << result << "\n";
        } else if (cmd == "2" && !args.empty()) {
            int *sorted = sort_ptr(args.data(), args.size());
            print_array(sorted, args.size());
            free(sorted);
        } else {
            std::cout << "Invalid command or arguments.\n";
        }
    }

    if (gcd_handle) dlclose(gcd_handle);
    if (sort_handle) dlclose(sort_handle);
    return 0;
}
