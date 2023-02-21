#ifndef BASE64_H
#define BASE64_H

#include <string>
#include <vector>

namespace Base64 {
    std::string encode(const std::string& buf);
    std::string encode(const std::vector<uint8_t>& buf);
    std::string encode(const uint8_t* buf, unsigned int bufLen);
    std::string decode(const std::string& encoded_string);
}

#endif /* BASE64_H */
