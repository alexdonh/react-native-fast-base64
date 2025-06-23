#ifndef BASE64_H
#define BASE64_H

#include <string>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace Base64 {
    // Main encoding function with optional URL-safe mode
    std::string encode(const uint8_t* data, size_t len, bool urlSafe = false);

    // Convenience overloads
    inline std::string encode(const std::string& s, bool urlSafe = false) {
        return s.empty() ? std::string() : encode(reinterpret_cast<const uint8_t*>(s.data()), s.size(), urlSafe);
    }

    inline std::string encode(const std::vector<uint8_t>& buf, bool urlSafe = false) {
        return buf.empty() ? std::string() : encode(buf.data(), buf.size(), urlSafe);
    }

    // Decoding function that handles both standard and URL-safe Base64
    std::string decode(const std::string& encoded);

    // No more NEON check needed
}

#endif /* BASE64_H */