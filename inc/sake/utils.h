#ifndef SAKE_UTILS_H
#define SAKE_UTILS_H

#include <stdint.h>

static inline uint32_t sake_utils_next_pow2(uint32_t v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;

    return v;
}

static inline uint32_t sake_utils_utf8_length(uint8_t utf8)
{
    if ((utf8 & 0x80) == 0x00) return 1;
    if ((utf8 & 0xE0) == 0xC0) return 2;
    if ((utf8 & 0xF0) == 0xE0) return 3;
    if ((utf8 & 0xF8) == 0xF0) return 4;
    return 0;
}

static inline uint32_t sake_utils_utf8_from_char(const char *data)
{
    if ((data[0] & 0x80) == 0x00) return ((uint8_t) data[0]);
    if ((data[0] & 0xE0) == 0xC0) return ((uint8_t) data[0]) | (((uint8_t) data[1]) << 8);
    if ((data[0] & 0xF0) == 0xE0) return ((uint8_t) data[0]) | (((uint8_t) data[1]) << 8) | (((uint8_t) data[2]) << 16);
    if ((data[0] & 0xF8) == 0xF0) return ((uint8_t) data[0]) | (((uint8_t) data[1]) << 8) | (((uint8_t) data[2]) << 16) | (((uint8_t) data[3]) << 24);
    return 0;
}

static inline uint32_t sake_utils_utf8_from_code_point(uint16_t code_point)
{

    if (code_point < 0x80) return ((uint8_t) code_point);
    if (code_point < 0x800) return ((uint8_t) (0xC0 | (code_point >> 6))) | (((uint8_t) (0x80 | (code_point & 0x3F))) << 8);
    return ((uint8_t) (0xE0 | (code_point >> 12))) | (((uint8_t) (0x80 | ((code_point >> 6) & 0x3F))) << 8) | (((uint8_t) (0x80 | (code_point & 0x3F))) << 16);
}

#endif /* SAKE_UTILS_H */