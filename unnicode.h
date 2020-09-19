#ifndef UNNICODE_H
#define UNNICODE_H

#include <cstdint>

namespace utf {
    int decode(std::uint8_t *in, std::uint32_t *cp, int len);
    bool inline is_contd_byte(std::uint8_t *byte);
}

#endif
