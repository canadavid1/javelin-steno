#include "../font.h"

const uint8_t GLYPH_DATA[200] = {
    0xfc, 0x3f, 0xfe, 0x7f, 0xff, 0xff, 0x07, 0xe0, 0x03, 0xc0, 0x03, 0xc0,
    0x07, 0xe0, 0xff, 0xff, 0xfe, 0x7f, 0xfc, 0x3f, 0x18, 0x00, 0x18, 0x00,
    0x1c, 0x00, 0x1e, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0c, 0xf0, 0x0e, 0xf8, 0x0f, 0xfc, 0x07, 0xfe,
    0x03, 0xef, 0x83, 0xe7, 0xc7, 0xe3, 0xff, 0xe1, 0xfe, 0xe0, 0x7c, 0xe0,
    0x0c, 0x30, 0x0e, 0x70, 0x0f, 0xf0, 0x07, 0xe0, 0x83, 0xc1, 0x83, 0xc1,
    0xc7, 0xe3, 0xff, 0xff, 0xfe, 0x7f, 0x7c, 0x3e, 0xc0, 0x07, 0xe0, 0x07,
    0xf0, 0x07, 0x78, 0x07, 0x3c, 0x07, 0x1e, 0x07, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x00, 0x07, 0xff, 0x31, 0xff, 0x71, 0xff, 0xf1, 0xc7, 0xe1,
    0xc7, 0xc1, 0xc7, 0xc1, 0xc7, 0xe3, 0x87, 0xff, 0x87, 0x7f, 0x07, 0x3f,
    0xf0, 0x3f, 0xfc, 0x7f, 0xfe, 0xff, 0x8f, 0xe3, 0xc7, 0xc1, 0xc3, 0xc1,
    0xc1, 0xe3, 0x80, 0xff, 0x00, 0x7f, 0x00, 0x3e, 0x03, 0x00, 0x03, 0x00,
    0x03, 0x00, 0x03, 0x00, 0x03, 0xfe, 0xc3, 0xff, 0xfb, 0xff, 0xff, 0x01,
    0x3f, 0x00, 0x0f, 0x00, 0x3c, 0x3c, 0x7e, 0x7e, 0xff, 0xff, 0xc7, 0xe3,
    0x83, 0xc1, 0x83, 0xc1, 0xc7, 0xe3, 0xff, 0xff, 0x7e, 0x7e, 0x3c, 0x3c,
    0x7c, 0x30, 0xfe, 0x70, 0xff, 0xf1, 0xc7, 0xe3, 0x83, 0xc3, 0x83, 0xc3,
    0xc7, 0xe1, 0xff, 0xff, 0xfe, 0x7f, 0xfc, 0x3f,
};

const uint16_t DATA_OFFSETS[10] = {
    1, 21, 41, 61, 81, 101, 121, 141, 161, 181,
};

const Font Font::MEDIUM_DIGITS = {
    0x30, // startUnicode
    0x3a, // endUnicode
    10,   // characterWidth
    16,   // height
    15,   // baseline
    1,    // spacing
    DATA_OFFSETS,
    GLYPH_DATA,
};