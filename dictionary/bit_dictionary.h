
/*
    The dictionary is a heavily optimized binary trie,
    with bits being packed as much as possible.
    Translations are stored with a huffman tree encoding,
    taking into account the last two characters as
    a-z, space, or anything else
    Keys have to be self-delimited, meaning that it is 
    obvious when a key ends and the data begins.

    All data is big-endian because it was e

    The trie is represented high-level as either
    * a bit, meaning that all keys have this bit in this position
    * a pointer, meaning that keys with a 1 here are at the pointer but keys 
      with a 0 are at the next location.
      Technically, they're not pointers but byte indicies from the start of the
      structure
    * data, binary data for the translation, must be self-delimited
    
    Representing:
        100 -> "a"
        0100 -> "b"
        011 -> "c"

        start -> [ptr 1 ptr 0 "b"]
                   |     |
                   |     +>["c"]
                   |
                   +>[0 0 "a"]

    Data is byte-oriented. Data is alternatingly a sequence of bits,
    and a pointer or data. Sequences of bits can be of any length from 0 to 255.
    Pointers are only 22 bits, so the top two bits are used by the preceeding 
    bit sequence.

    length  bytes
    0       10
    1       0x
    2       11111110 xx
    3       1111110x xx
    ...
    6       1110xxxx xx
    7+      110nnnnn 11111110 <n bytes of xxxxxxxx> xx
                     1111110x
                     111110xx
                     11110xxx
                     1110xxxx
                     110xxxxx
                     10xxxxxx
                     0xxxxxxx
    
    After this comes a pointer that is a total of 22 bits, representing
    a byte address for the branch. This is the reason for the two bits
    all the encodings end in.

    Translations therefore start at bit 5, and go downwards after this.

    Because I wanted to, and because it is easier to visualize, data
    is MSB first, and big-endian.
    
*/

#pragma once
#include "dictionary.h"
#include "dictionary_definition.h"

class StenoBitDictionary final : public StenoDictionary {
public:
    StenoBitDictionary(const StenoDictionaryDefinition &definition)
        : StenoDictionary(definition.maximumOutlineLength)
        , definition(definition)
        {}
    virtual StenoDictionaryLookupResult
    Lookup(const StenoDictionaryLookup &lookup) const;
    using StenoDictionary::Lookup;

    virtual const StenoDictionary *
    GetLookupProvider(const StenoDictionaryLookup &lookup) const;

    virtual bool
    ReverseMapDictionaryLookup(StenoReverseMapDictionaryLookup &lookup) const = 0;

    virtual size_t GetMaximumOutlineLength() const;
    virtual const char *GetName() const;
    virtual void PrintInfo(int depth) const;
    virtual bool PrintDictionary(bool hasData) const;

private:
    struct {
        StenoDictionaryLookup lookup{nullptr,0};
        char *translation=nullptr;
    } data[16];
    const StenoDictionaryDefinition &definition;
};

class HuffmanTreeEncodingData
{
    using u8 = uint8_t;
public:
    size_t Translate(const u8* ptr) // starts from bit 5, works downwards.
    {
        
    }
private:
    
};