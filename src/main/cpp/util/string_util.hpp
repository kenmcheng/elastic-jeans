#ifndef _ELASTICJEANS_STRING_UTIL_H
#define _ELASTICJEANS_STRING_UTIL_H
#include <log/log.hpp>

#include <vector>
#include <string>

namespace elasticJeans {


inline std::vector<std::string> tokenize(const std::string& s,
                                const std::string& delimiter, 
                                bool allowEmpty = false,
                                 size_t begin = 0, 
                                 size_t end = std::string::npos) {
    if (delimiter.size() == 0)  return {s};
    int sz = s.size();
    std::vector<std::string> tokens;
    int found = -delimiter.size();
    while ((found = s.find(delimiter, begin)) < end) {
        if (!allowEmpty && found == begin) {
            begin = found + delimiter.size();
            continue;
        }
        tokens.push_back(std::move(s.substr(begin, found - begin)));
        begin = found + delimiter.size();
    }
    if (begin < sz)
        tokens.push_back(std::move(s.substr(begin, sz - begin)));

    return tokens;
}

} // namespace elasticJeans

#endif