//---------------------------------------------------------------------------

#pragma once
#include "dictionary.h"
#include "dictionary_definition.h"

//---------------------------------------------------------------------------

class StenoCompactMapDictionary final : public StenoDictionary {
public:
  StenoCompactMapDictionary(const StenoDictionaryDefinition &definition)
      : StenoDictionary(definition.maximumOutlineLength),
        textBlock(definition.textBlock), definition(definition),
        strokes(CreateStrokeCache(definition)) {}

  virtual StenoDictionaryLookupResult
  Lookup(const StenoDictionaryLookup &lookup) const;
  using StenoDictionary::Lookup;

  virtual const StenoDictionary *
  GetLookupProvider(const StenoDictionaryLookup &lookup) const;

  virtual bool
  ReverseMapDictionaryLookup(StenoReverseMapDictionaryLookup &lookup) const;

  virtual size_t GetMaximumOutlineLength() const;
  virtual const char *GetName() const;
  virtual void PrintInfo(int depth) const;
  virtual bool PrintDictionary(bool hasData) const;

private:
  const uint8_t *textBlock;
  const StenoDictionaryDefinition &definition;

  // This is offset by 1 to simplify lookup code marginally.
  const StenoMapDictionaryStrokesDefinition *strokes;

  static const StenoMapDictionaryStrokesDefinition *
  CreateStrokeCache(const StenoDictionaryDefinition &definition);
};

//---------------------------------------------------------------------------
