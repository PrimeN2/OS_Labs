#include <unistd.h>

int is_prime(int n) {
    if (n < 2) return 0;

    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return 0;
        }
    }

    return 1;
}

int custom_atoi(const char *str, int *num) {
    int result = 0;
    int sign = 1;
    int i = 0;

    if (str[0] == '-') {
        sign = -1;
        i = 1;
    }

    while (str[i] != '\n') {
        if (str[i] < '0' || str[i] > '9') return 0;
        result = result * 10 + (str[i] - '0');
        i++;
    }

    *num = sign * result;
    return 1;
}

int main () {
    char buffer[1024];
    int pos = 0;

    while (1) {
        char curr;
        int n = read(0, &curr, 1);
        if (n <= 0) break;

        if (curr == '\n') {
            buffer[pos] = '\n';
            pos++;
            int num;

            if (!custom_atoi(buffer, &num)) {
                write(2, "Invalid number\n", 15);
                _exit(1);
            }

            if (num < 0) {
                _exit(0);
            }

            if (is_prime(num)) {
                write(1, buffer, pos);
            }
            else {
                _exit(0);
            }

            pos = 0;
        }
        else {
            buffer[pos] = curr;
            pos++; 
        }
    }
    
    return 0;
}