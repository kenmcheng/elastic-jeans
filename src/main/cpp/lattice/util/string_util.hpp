#ifndef _LATTICE_STRING_UTIL_H
#define _LATTICE_STRING_UTIL_H
#include <lattice/log/log.hpp>

#include <vector>
#include <string>
#include <algorithm>

namespace lattice {

inline std::vector<std::string> tokenize(const std::string& s,
                                const std::string& delimiter, 
                                bool allowEmpty = false,
                                bool trim = true,
                                size_t begin = 0, 
                                size_t end = std::string::npos) {
    if (delimiter.size() == 0)  return {s};
    size_t sz = s.size();
    std::vector<std::string> tokens;
    if (!sz) 
        return tokens;

    int found = -delimiter.size();
    while ((found = s.find(delimiter, begin)) < end) {
        int cutoff = found;
        while (trim && begin < cutoff && s[begin] == ' ')
            begin++;
        while (trim && cutoff > begin && s[cutoff-1] == ' ')
            cutoff--;
        if (!allowEmpty && cutoff == begin) {
            begin = found + delimiter.size();
            continue;
        }
        tokens.push_back(std::move(s.substr(begin, cutoff - begin)));
        begin = found + delimiter.size();
    }
    if (begin < sz && begin < end) {
        int cutoff = std::min(sz, end);
        while (trim && begin < cutoff && s[begin] == ' ')
            begin++;
        while (trim && cutoff > begin && s[cutoff-1] == ' ')
            cutoff--;

        if (allowEmpty || cutoff != begin) 
            tokens.push_back(std::move(s.substr(begin, cutoff - begin)));
    }

    return tokens;
}

inline std::string& ftrim(std::string& s, const char target = ' ') {
    size_t sz = s.size();
    int cnt = 0;
    for (auto c : s) {
        if (c == ' ') cnt++;
    }
    if (!cnt) return s;
    if (cnt == sz) {
        s.clear();
        return s;
    }
    for (int i = cnt, ptr = 0; i < sz; i++,ptr++) {
        std::swap(s[i], s[ptr]);
    }
    while (cnt-- > 0) s.pop_back();
    return s;
} 

inline std::string& rtrim(std::string& s, const char target = ' ') {
    while (s.size() && s.back() == target)
        s.pop_back();

    return s;
}

inline std::string& trim(std::string& s, const char target = ' ') {
    ftrim(s, target);
    rtrim(s, target);
    return s;
}

} // namespace lattice

#endif