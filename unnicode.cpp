#include "unnicode.h"

bool utf8::is_contd_byte (std::uint8_t *byte) {
    return (*byte & 0x80) == 0x80 && (*byte & 0x40) == 0;
}

void utf8::append_contd_byte(std::uint8_t *byte, std::uint32_t *cp) {
    *cp <<= 6;
    *cp |= 0x3F & *byte;
}

int utf8::decode (std::uint8_t *in, std::uint32_t *cp, int len) {
    int bytes_decoded = 0;
    std::uint32_t *begin = cp;
    while (bytes_decoded < len) {
        /* One byte utf-8 encoded sequence */
        if ((*in & 0x80) == 0){
            *cp = 0 | *in;
            cp++; in++; bytes_decoded++;
        }
        /* Two byte utf-8 encoded sequence */
        else if ((*in & 0xC0) == 0xC0 && (*in & 0x20) == 0 &&
                bytes_decoded+1 < len &&
                utf8::is_contd_byte(in+1)) {

            *cp = 0x0000001F & *in;
            append_contd_byte(in+1, cp);
            cp++; in+=2; bytes_decoded+=2;
        }
        /* Three byte utf-8 encoded sequence */
        else if ((*in & 0xE0) == 0xE0 && (*in & 0x10) == 0 &&
                bytes_decoded+2 < len &&
                utf8::is_contd_byte(in+1) && utf8::is_contd_byte(in+2)) {

            *cp = 0x0000000F & *in;
            for (int i = 1; i < 3; i++) append_contd_byte(in+i, cp);
            cp++; in+=3; bytes_decoded+=3;
        }
        /* Four byte utf-8 encoded sequence */
        else if ((*in & 0xF0) == 0xF0 && (*in & 0x08) == 0 &&
                bytes_decoded+3 < len &&
                utf8::is_contd_byte(in+1) && utf8::is_contd_byte(in+2) && utf8::is_contd_byte(in+3)){

            *cp = 0x00000007 & *in;
            for (int i = 1; i < 4; i++) append_contd_byte(in+i, cp);
            cp++; in+=4; bytes_decoded+=4;
        }
        else {
            *cp++ = 0xFFFD;
            in++;
            bytes_decoded++;
        }
    }
    return cp-begin;
}
