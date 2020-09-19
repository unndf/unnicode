#ifndef UNNICODE_H
#define UNNICODE_H

#include <cstdint>

namespace utf8 {
    bool inline is_contd_byte(std::uint8_t *byte);
    void inline append_contd_byte(std::uint8_t *byte, std::uint32_t *cp);
    int decode(std::uint8_t *in, std::uint32_t *cp, int len);
}

#endif
