#include "Base64.h"

namespace Base64 {
    static const uint8_t from_base64[128] = {
        // 8 rows of 16 = 128
        // Note: only requires 123 entries, as we only lookup for <= z , which z=122
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  62, 255,  62, 255,  63,
        52,   53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255,   0, 255, 255, 255,
        255,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
        15,   16,  17,  18,  19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255,  63,
        255,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,
        41,   42,  43,  44,  45,  46,  47,  48,  49,  50,  51, 255, 255, 255, 255, 255
    };

    static const char to_base64[65] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

    std::string encode(const std::string& buf)
    {
        if (buf.empty())
            return encode(nullptr, 0);
        else
            return encode(reinterpret_cast<uint8_t const*>(&buf[0]), buf.size());
    }

    std::string encode(const std::vector<uint8_t>& buf)
    {
        if (buf.empty())
            return encode(nullptr, 0);
        else
            return encode(&buf[0], buf.size());
    }

    std::string encode(const uint8_t* buf, unsigned int bufLen)
    {
        // Calculate how many bytes that needs to be added to get a multiple of 3
        size_t missing = 0;
        size_t ret_size = bufLen;
        while ((ret_size%3)!=0)
        {
            ++ret_size;
            ++missing;
        }

        // Expand the return string size to a multiple of 4
        ret_size = 4*ret_size/3;

        std::string ret;
        ret.reserve(ret_size);

        for (unsigned int i=0; i<ret_size/4; ++i)
        {
            // Read a group of three bytes (avoid buffer overrun by replacing with 0)
            const size_t index = i*3;
            const uint8_t b3_0 = (index+0 < bufLen) ? buf[index+0] : 0;
            const uint8_t b3_1 = (index+1 < bufLen) ? buf[index+1] : 0;
            const uint8_t b3_2 = (index+2 < bufLen) ? buf[index+2] : 0;

            // Transform into four base 64 characters
            const uint8_t b4_0 =                        ((b3_0 & 0xfc) >> 2);
            const uint8_t b4_1 = ((b3_0 & 0x03) << 4) + ((b3_1 & 0xf0) >> 4);
            const uint8_t b4_2 = ((b3_1 & 0x0f) << 2) + ((b3_2 & 0xc0) >> 6);
            const uint8_t b4_3 = ((b3_2 & 0x3f) << 0);

            // Add the base 64 characters to the return value
            ret.push_back(to_base64[b4_0]);
            ret.push_back(to_base64[b4_1]);
            ret.push_back(to_base64[b4_2]);
            ret.push_back(to_base64[b4_3]);
        }

        // Replace data that is invalid (always as many as there are missing bytes)
        for (size_t i=0; i<missing; ++i)
            ret[ret_size - i - 1] = '=';

        return ret;
    }

    std::string decode(const std::string& encoded_string)
    {
        // Make sure the *intended* string length is a multiple of 4
        size_t encoded_size = encoded_string.size();

        while ((encoded_size % 4) != 0)
            ++encoded_size;

        const size_t n = encoded_string.size();
        std::string ret;
        ret.reserve(3*encoded_size/4);

        for (size_t i=0; i<encoded_size; i += 4)
        {
            // Note: 'z' == 122

            // Get values for each group of four base 64 characters
            const uint8_t b4_0 = (            encoded_string[i+0] <= 'z') ? from_base64[encoded_string[i+0]] : 0xff;
            const uint8_t b4_1 = (i+1 < n and encoded_string[i+1] <= 'z') ? from_base64[encoded_string[i+1]] : 0xff;
            const uint8_t b4_2 = (i+2 < n and encoded_string[i+2] <= 'z') ? from_base64[encoded_string[i+2]] : 0xff;
            const uint8_t b4_3 = (i+3 < n and encoded_string[i+3] <= 'z') ? from_base64[encoded_string[i+3]] : 0xff;

            // Transform into a group of three bytes
            const uint8_t b3_0 = ((b4_0 & 0x3f) << 2) + ((b4_1 & 0x30) >> 4);
            const uint8_t b3_1 = ((b4_1 & 0x0f) << 4) + ((b4_2 & 0x3c) >> 2);
            const uint8_t b3_2 = ((b4_2 & 0x03) << 6) + ((b4_3 & 0x3f) >> 0);

            // Add the byte to the return value if it isn't part of an '=' character (indicated by 0xff)
            if (b4_1 != 0xff) ret.push_back(b3_0);
            if (b4_2 != 0xff) ret.push_back(b3_1);
            if (b4_3 != 0xff) ret.push_back(b3_2);
        }

        return ret;
    }
}
