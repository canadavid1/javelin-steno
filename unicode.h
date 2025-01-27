//---------------------------------------------------------------------------

#pragma once
#include <stdint.h>

//---------------------------------------------------------------------------

class Unicode {
public:
  static uint32_t ToUpper(uint32_t c);
  static uint32_t ToLower(uint32_t c);

  static bool IsAsciiDigit(uint32_t c) { return '0' <= c && c <= '9'; }
  static bool IsLetter(uint32_t c);

#if JAVELIN_CPU_CORTEX_M0 || JAVELIN_CPU_CORTEX_M4
  static bool IsWhitespace(uint32_t c) {
    return c == ' ' || int32_t(0x807c0000u << c) < 0;
  }
#else
  static bool IsWhitespace(uint32_t c);
#endif

  static bool IsWordCharacter(uint32_t c);
};

//---------------------------------------------------------------------------
