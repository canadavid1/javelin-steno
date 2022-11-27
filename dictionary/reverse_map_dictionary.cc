//---------------------------------------------------------------------------

#include "reverse_map_dictionary.h"
#include "../str.h"

//---------------------------------------------------------------------------

StenoDictionaryLookupResult
StenoReverseMapDictionary::Lookup(const StenoDictionaryLookup &lookup) const {
  return dictionary->Lookup(lookup);
}

const StenoDictionary *StenoReverseMapDictionary::GetLookupProvider(
    const StenoDictionaryLookup &lookup) const {
  return dictionary->GetLookupProvider(lookup);
}

void StenoReverseMapDictionary::ReverseLookup(
    StenoReverseDictionaryLookup &result) const {
  StenoReverseDictionaryLookup value(result.strokeThreshold, result.lookup);

  dictionary->ReverseLookup(value);
  AddMapDictionaryResults(value);
  AddValidLookupProviders(result, value);
}

void StenoReverseMapDictionary::AddMapDictionaryResults(
    StenoReverseDictionaryLookup &result) const {
  const uint8_t *left = textBlock + 1;
  const uint8_t *right = textBlock + textBlockLength;

  while (left < right) {
    const uint8_t *mid = left + (right - left) / 2;

    const uint8_t *wordStart = mid;
    while (wordStart[-1] != 0xff) {
      --wordStart;
    }

    int compare = strcmp(result.lookup, (const char *)wordStart);
    if (compare < 0) {
      right = wordStart;
    } else if (compare > 0) {
      while (*wordStart != 0xff) {
        ++wordStart;
      }
      left = wordStart + 1;
    } else {
      const uint8_t *p = wordStart;
      while (*p != 0) {
        ++p;
      }
      ++p;
      while (*p != 0xff) {
        uint32_t offset = p[0] | (p[1] << 7) | (p[2] << 14) + (p[3] << 21);
        ReverseMapDictionaryLookup(result, baseAddress + offset);
        p += 4;
      }

      return;
    }
  }
}

void StenoReverseMapDictionary::AddValidLookupProviders(
    StenoReverseDictionaryLookup &result,
    StenoReverseDictionaryLookup &value) const {
  const StenoChord *chords = value.chords;
  for (size_t i = 0; i < value.resultCount; ++i) {
    size_t length = value.resultLengths[i];
    const StenoDictionary *lookupProvider = value.lookupProviders[i];
    if (dictionary->GetLookupProvider(chords, length) == lookupProvider) {
      result.AddResult(chords, length, lookupProvider);
    }
    chords += length;
  }
}

bool StenoReverseMapDictionary::ReverseMapDictionaryLookup(
    StenoReverseDictionaryLookup &result, const void *data) const {
  return dictionary->ReverseMapDictionaryLookup(result, data);
}

unsigned int StenoReverseMapDictionary::GetMaximumMatchLength() const {
  return dictionary->GetMaximumMatchLength();
}

const char *StenoReverseMapDictionary::GetName() const {
  return "#internal#reverse_map_dictionary";
}

void StenoReverseMapDictionary::PrintInfo(int depth) const {
  dictionary->PrintInfo(depth);
}

bool StenoReverseMapDictionary::PrintDictionary(bool hasData) const {
  return dictionary->PrintDictionary(hasData);
}

void StenoReverseMapDictionary::ListDictionaries() const {
  dictionary->ListDictionaries();
}

bool StenoReverseMapDictionary::EnableDictionary(const char *name) {
  return dictionary->EnableDictionary(name);
}

bool StenoReverseMapDictionary::DisableDictionary(const char *name) {
  return dictionary->DisableDictionary(name);
}

bool StenoReverseMapDictionary::ToggleDictionary(const char *name) {
  return dictionary->ToggleDictionary(name);
}

//---------------------------------------------------------------------------