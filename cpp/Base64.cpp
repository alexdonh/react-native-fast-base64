#include "Base64.h"
#include <string>
#include <vector>
#include <cstring>

namespace Base64 {

    static constexpr char std_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    static constexpr char url_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

    // Fast decode table: 255 = invalid
    static const uint8_t decode_table[256] = {
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  62, 255, 255, 255,  63,
         52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255, 255,  64, 255, 255,
        255,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
         15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255, 255,
        255,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,
         41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51, 255, 255, 255, 255, 255,
        // rest is 255
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
    };

    std::string encode(const uint8_t* data, size_t len, bool urlSafe) {
        if (!data || len == 0) return {};

        const char* chars = urlSafe ? url_chars : std_chars;
        size_t outLen = ((len + 2) / 3) * 4;
        std::string out;
        out.reserve(outLen);

        size_t i = 0;
        for (; i + 2 < len; i += 3) {
            uint32_t v = (static_cast<uint32_t>(data[i]) << 16) |
                         (static_cast<uint32_t>(data[i + 1]) << 8) |
                          static_cast<uint32_t>(data[i + 2]);
            out.push_back(chars[(v >> 18) & 0x3F]);
            out.push_back(chars[(v >> 12) & 0x3F]);
            out.push_back(chars[(v >> 6) & 0x3F]);
            out.push_back(chars[v & 0x3F]);
        }

        // Handle tail
        if (i < len) {
            uint32_t v = static_cast<uint32_t>(data[i]) << 16;
            out.push_back(chars[(v >> 18) & 0x3F]);
            if (i + 1 < len) {
                v |= static_cast<uint32_t>(data[i + 1]) << 8;
                out.push_back(chars[(v >> 12) & 0x3F]);
                out.push_back(chars[(v >> 6) & 0x3F]);
                out.push_back('=');
            } else {
                out.push_back(chars[(v >> 12) & 0x3F]);
                out.push_back('=');
                out.push_back('=');
            }
        }

        // Remove padding if urlSafe
        if (urlSafe) {
            while (!out.empty() && out.back() == '=') out.pop_back();
        }

        return out;
    }

    std::string decode(const std::string& encoded) {
        size_t len = encoded.size();
        // Strip any trailing padding
        while (len > 0 && (encoded[len - 1] == '=' || encoded[len - 1] == '.')) --len;
        if (len == 0) return {};

        size_t out_len = (len * 3) / 4;
        std::string result;
        result.reserve(out_len);

        size_t i = 0;
        while (i + 4 <= len) {
            uint8_t a = decode_table[static_cast<uint8_t>(encoded[i])];
            uint8_t b = decode_table[static_cast<uint8_t>(encoded[i + 1])];
            uint8_t c = decode_table[static_cast<uint8_t>(encoded[i + 2])];
            uint8_t d = decode_table[static_cast<uint8_t>(encoded[i + 3])];

            if (a == 255 || b == 255 || c == 255 || d == 255) return {};

            uint32_t triple = (a << 18) | (b << 12) | (c << 6) | d;
            result.push_back((triple >> 16) & 0xFF);
            result.push_back((triple >> 8) & 0xFF);
            result.push_back(triple & 0xFF);

            i += 4;
        }

        // Tail: up to 3 bytes
        if (i < len) {
            uint8_t a = decode_table[static_cast<uint8_t>(encoded[i])];
            if (a == 255) return {};
            uint8_t b = (i + 1 < len) ? decode_table[static_cast<uint8_t>(encoded[i + 1])] : 0;
            if ((i + 1 < len) && b == 255) return {};

            uint8_t c = (i + 2 < len) ? decode_table[static_cast<uint8_t>(encoded[i + 2])] : 0;
            if ((i + 2 < len) && c == 255) return {};

            // At least two base64 chars required for 1 byte
            result.push_back((a << 2) | (b >> 4));
            if (i + 2 < len) {
                // 2 more base64 chars for a second byte
                result.push_back(((b & 0x0F) << 4) | (c >> 2));
                if (i + 3 < len) {
                    uint8_t d = decode_table[static_cast<uint8_t>(encoded[i + 3])];
                    if (d == 255) return {};
                    result.push_back(((c & 0x03) << 6) | d);
                }
            }
        }
        return result;
    }
}
