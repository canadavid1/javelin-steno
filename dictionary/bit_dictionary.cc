



#include "bit_dictionary.h"
#include "../bit.h"
#include "../console.h"
#include "../str.h"
#include "../uint24.h"


StenoDictionaryLookupResult
StenoBitDictionary::Lookup(const StenoDictionaryLookup &lookup) const {
  // placeholder for actual logic
  for(auto i : data)
  {
    if (i.lookup.length != lookup.length) continue;
    for(int j = 0; j < lookup.length; j++)
      if (i.lookup.strokes[j]!=lookup.strokes[j]) goto next_i;
    return StenoDictionaryLookupResult::CreateStaticString(i.translation);
  next_i:;
  }
  return StenoDictionaryLookupResult::CreateInvalid();
}
const StenoDictionary *StenoBitDictionary::GetLookupProvider(
    const StenoDictionaryLookup &lookup) const {
  if (Lookup(lookup).IsValid()) return this;
  return nullptr;
}
bool StenoBitDictionary::ReverseMapDictionaryLookup(
    StenoReverseMapDictionaryLookup &lookup) const {
  return false; // TODO
}

const char *StenoBitDictionary::GetName() const {return definition.name;}
void StenoBitDictionary::PrintInfo(int depth) const {
  Console::Printf("This is not implemented yet.");
}

