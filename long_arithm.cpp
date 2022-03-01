const int BASE = 10;
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
class bigint {
private:
    std::vector<long long> number;
    uint32_t degree = 0;

public:
    void remove_zero() {
        while (number.size() > 1 && number.back() == 0) {
            number.pop_back();
        }
    }
    bigint() {
        uint32_t temp_base = BASE;
        while (temp_base > 1) {
            temp_base /= 10;
            degree++;
        }
        number.push_back(0);
    }

    explicit bigint(const std::string &str) {
        uint32_t temp_base = BASE;
        while (temp_base > 1) {
            temp_base /= 10;
            degree++;
        }
        if (!str.empty()) {
            for (auto i = static_cast<long long>(str.length()); i > 0;
                 i -= degree) {
                if (i < degree) {
                    number.push_back(atoi(str.substr(0, i).c_str()));
                } else {
                    number.push_back(
                        atoi(str.substr(i - degree, degree).c_str()));
                }
            }

            remove_zero();
        }
    }
    // cppcheck-suppress noExplicitConstructor
    bigint(unsigned long long number_to) {
        uint32_t temp_base = BASE;
        while (temp_base > 1) {
            temp_base /= 10;
            degree++;
        }

        do {
            this->number.push_back(static_cast<long long>(number_to) % BASE);
            number_to /= BASE;
        } while (number_to != 0);
    }

    [[nodiscard]] std::string to_string() const {
        std::stringstream stringstr;
        stringstr << *this;
        return stringstr.str();
    }

    explicit operator unsigned int() const {
        unsigned int result = 0;
        for (auto i = static_cast<long long>(number.size() - 1);; i--) {
            result *= static_cast<unsigned int>(BASE);
            result += static_cast<unsigned int>(number[i]);
            if (i == 0) {
                break;
            }
        }
        return result;
    }

    friend bool operator==(const bigint &a, const bigint &b) {
        return (!(a > b) && !(b > a));
    }

    friend bool operator>(const bigint &a, const bigint &b) {
        if (a.number.size() != b.number.size()) {
            return a.number.size() > b.number.size();
        } else {
            for (long long i = static_cast<long long>(b.number.size()) - 1;
                 i >= 0; --i) {
                if (a.number[i] != b.number[i]) {
                    return a.number[i] > b.number[i];
                }
            }

            return false;
        }
    }

    friend bool operator<(const bigint &a, const bigint &b) {
        return (!(a > b) && !(a == b));
    }

    friend bool operator!=(const bigint &a, const bigint &b) {
        return (!(operator==(a, b)));
    }

    friend bool operator>=(const bigint &a, const bigint &b) {
        return (!operator<(a, b));
    }

    friend bool operator<=(const bigint &a, const bigint &b) {
        return (!operator>(a, b));
    }

    friend bigint operator+(const bigint &a, const bigint &b) {
        bigint result = a;
        result += b;
        return result;
    }

    bigint &operator+=(const bigint &b) {
        int remainder = 0;
        for (size_t i = 0; i < std::max(this->number.size(), b.number.size()) ||
                           remainder != 0;
             ++i) {
            if (i == this->number.size()) {
                this->number.push_back(0);
            }
            if (i < b.number.size()) {
                this->number[i] += (remainder + b.number[i]);
            } else {
                this->number[i] += remainder;
            }
            if (this->number[i] >= BASE) {
                remainder = 1;
            } else {
                remainder = 0;
            }
            if (remainder != 0) {
                this->number[i] -= BASE;
            }
        }
        return *this;
    }

    bigint &operator-=(const bigint &b) {
        int remainder = 0;
        for (size_t i = 0; i < b.number.size() || remainder != 0; ++i) {
            if (i < b.number.size()) {
                this->number[i] -= (remainder + b.number[i]);
            } else {
                this->number[i] -= remainder;
            }
            if (this->number[i] < 0) {
                remainder = 1;
            } else {
                remainder = 0;
            }
            if (remainder != 0) {
                this->number[i] += BASE;
            }
        }

        remove_zero();

        return *this;
    }

    friend bigint operator-(const bigint &a, const bigint &b) {
        bigint result = a;
        result -= b;
        return result;
    }
    bigint &operator++() {
        *this += 1;
        return *this;
    }

    bigint operator++(int) {
        bigint result = *this;
        *this += 1;
        return result;
    }
    bigint operator--(int) {
        bigint result = *this;
        *this -= 1;
        return result;
    }

    bigint &operator--() {
        *this -= 1;
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &out, const bigint &a) {
        if (a.number.empty()) {
            out << 0;
        } else {
            out << a.number.back();
            char filling = out.fill('0');
            for (long long i = static_cast<long long>(a.number.size()) - 2;
                 i >= 0; --i) {
                out << std::setw(static_cast<int>(a.degree)) << a.number[i];
            }

            out.fill(filling);
        }
        return out;
    }

    friend std::istream &operator>>(std::istream &in, bigint &a) {
        std::string instr;
        in >> instr;
        a = bigint(instr);
        return in;
    }
};
