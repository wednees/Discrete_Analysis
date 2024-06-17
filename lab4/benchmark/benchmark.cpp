#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include <ctime>

std::pair<int, int> position_getter(int number, const std::vector<int> &string_lengths) {
    int string_number = 0;
    while (number - string_lengths[string_number] > 0) {
        number -= string_lengths[string_number];
        ++string_number;
    }
    ++string_number;
    return {string_number, number};
}

std::pair<int, int> GetStringNumber(int number, const std::vector<int> &string_lengths)
{
    int string_number = 0;
    while (number - string_lengths[string_number] > 0)
    {
        number -= string_lengths[string_number];
        ++string_number;
    }
    ++string_number;
    return {string_number, number};
}

std::map<std::string, std::vector<int>> bad_symbol_prerocess(std::vector<std::string> &pattern) {
    std::map<std::string, std::vector<int>> result;
    for (int i = pattern.size() - 1; i >= 0; --i) {
        result[pattern[i]].push_back(i);
    }
    return result;
}

std::vector<int> z_function(const std::vector<std::string> &s) {
    int n = s.size();
    std::vector<int> z(n);
    int left = 0, right = 0;

    for (int i = 1; i < n; ++i) {
        if (i <= right)
            z[i] = std::min(right - i + 1, z[i - left]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]])
            ++z[i];
        if (i + z[i] - 1 > right) {
            left = i;
            right = i + z[i] - 1;
        }
    }

    return z;
}

std::vector<int> n_function(std::vector<std::string> s) {
    std::reverse(s.begin(), s.end());
    std::vector<int> z = z_function(s), n(s.size());
    for (int i = 1; i < z.size(); ++i) {
        n[z.size() - i - 1] = z[i];
    }
    return n;
}

std::vector<int> l_fuction(const std::vector<int> &n) {
    std::vector<int> l(n.size());
    for (int i = 0; i < n.size(); ++i) {
        if (n[i]) {
            l[n.size() - n[i]] = i;
        }
    }
    return l;
}


int good_suffix(const std::vector<int> &l, int i) {
    if (l.size() > i && l[i]) {
        return l.size() - l[i];
    }
    return 0;
}


int bad_symbol(std::map<std::string, std::vector<int>> &table, const std::string &c, int pos) {
    if (!table[c].empty()) {
        for (auto elem: table[c]) {
            if (elem < pos) return pos - elem;
        }
    }
    return 1;
}


std::vector<int> Search(std::vector<std::string> &text, std::vector<std::string> &pattern) {


    auto N = n_function(pattern);
    auto L = l_fuction(N);
    auto table = bad_symbol_prerocess(pattern);
    
    int h = pattern.size() - 1;
    std::vector<int> M(text.size(), -1);
    std::vector<int> positions;
    while (h < text.size()) {
        bool flag = true;
        int position_to_stop = h - pattern.size();
        std::string mismatched;
        int i = pattern.size() - 1;
        for (int j = h; j > position_to_stop;) {
            if (M[j] == -1 || ((M[j] == 0) && (N[i]) == 0)) {
                if (text[j] == pattern[i]) { // 1st case
                    if (i > 0) {
                        --i;
                        --j;
                    } else {
                        break;
                    }
                } else {
                    mismatched = text[j];
                    M[h] = h - j;
                    flag = false;
                    break;
                }
            } else if (M[j] < N[i] && M[j]) {   // 2nd case
                j -= M[j];
                i -= M[j];
            } else if (M[j] == N[i] && M[j]) {  // 3rd case
                if (i == N[i]) {
                    M[h] = h - j;
                    break;
                }
                i -= M[j];
                j -= M[j];
            } else if (M[j] > N[i]) {           // 4th case
                if (i == N[i]) {
                    M[h] = h - j;
                    break;
                } else if (N[i] < i) {
                    mismatched = pattern[i - N[i] - 1];
                    M[h] = h - j;
                    flag = false;
                    break;
                } else {
                    break;
                }
            } else { //  5th case
                flag = false;
                break;
            }
        }
        if (flag) {
            positions.push_back(h - pattern.size() + 1);
            ++h;
        } else {
            int bad_symbol_res = bad_symbol(table, mismatched, i);
            int good_suffix_res = good_suffix(L, h);
            h += std::max(bad_symbol_res, std::max(1, good_suffix_res));
        }
    }
    return positions;
}

std::vector<int> NaiveSearch(std::vector<std::string> text, std::vector<std::string> pattern)
{
    std::vector<int> positions;
    for (int i = 0; i < text.size() - pattern.size(); ++i)
    {
        int j = i;
        for (int k = 0; k < pattern.size(); ++k)
        {
            if (pattern[k] != text[j])
            {
                break;
            }
            else if (pattern[k] == text[j] && k + 1 == pattern.size())
            {
                positions.push_back(i);
            }
            else
            {
                ++j;
            }
        }
    }
    return positions;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    // unsigned int start_time = clock();
    std::string string, pstring;
    std::vector<std::string> pattern, text;
    std::vector<int> string_lengths;
    std::getline(std::cin, pstring);
    std::stringstream pattern_stream(pstring);
    while (pattern_stream >> pstring) {
        std::transform(pstring.begin(), pstring.end(),
                    pstring.begin(), tolower);
        pattern.push_back(pstring);
    }

    while (std::getline(std::cin, string)) {
        std::stringstream text_stream(string);
        int string_len = 0;
        while (text_stream >> string) {
            ++string_len;
            std::transform(string.begin(), string.end(),
                        string.begin(), tolower);
            text.push_back(string);
        }
        string_lengths.push_back(string_len);
    }

    {
        unsigned int start_time = clock();
        std::vector<int> res = Search(text, pattern);

        for (auto &r: res) {
            auto p = position_getter(r + 1, string_lengths);
            // std::cout << p.first << ", " << p.second << "\n";
        }
        unsigned int end_time = clock();
        unsigned int search_time = end_time - start_time;
        std::cout << "AG: " << ((float)search_time) / CLOCKS_PER_SEC << "s" << std::endl;
    }

    {
        unsigned int start_time = clock();
        std::vector<int> res = NaiveSearch(text, pattern);
        for (auto &r : res)
        {
            auto p = GetStringNumber(r + 1, string_lengths);
            // std::cout << p.first << ", " << p.second << "\n";
        }
        unsigned int end_time = clock();
        unsigned int search_time = end_time - start_time;
        std::cout << "Naive: " << ((float)search_time) / CLOCKS_PER_SEC << "s" << std::endl;
    }
    return 0;
}