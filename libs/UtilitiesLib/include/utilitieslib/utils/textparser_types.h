#ifndef TEXTPARSER_TYPES_H
#define TEXTPARSER_TYPES_H

#include "../stdtypes.h"

// Public records used to author and export parser tables.
typedef U32 StructTypeField;   // ok, we're going to have to extend to 64-bits soon
typedef U32 StructFormatField; // fine for 32 bits here

// MAK 5/4/6 - OK, this is the way parse infos are going to be rearranged for future expansion
// and compatibility with all struct stuff:
//
//        name            char* (pointer-size)                    when <name> is hit in a text file, parse this token
//        type            U32, expandable to 64                    divided into 4 8-bit fields-
//                        primitive type:8                        int, string, struct, etc.
//                        options:8                                options for parsing like REDUNDANT_NAME, STRUCT_PARAM
//                        storage:8                                array, single item, fixed array, any options for allocation
//                        precision:8                                precision of binary storage/network for numbers
//        offset            size_t (pointer-size)                    offset into parent struct to store this token
//        param            intptr_t (pointer-size)                    for structs: size of struct
//                                                                for fixed arrays: number of elements
//                                                                for numbers: default value (only integer default allowed)
//                                                                for direct, single string: embedded string length
//                                                                for other strings: pointer to default value
//                                                                -- use interpretfield to find out what is held here
//        subtable        void* (pointer-size)                    for complex data types: pointer to subtable or other definition
//                                                                for primitives: pointer to StaticDefine list for string substitution
//                                                                -- use interpretfield to find out what is held here
//        format            U32, expandable to 64                    probably divided into 8-bit fields, only 2 fields right now-
//                        pretty print:8                            flags for how to print dates nicely, etc
//                        listview width:8                        width when using listview or the ui to display table
//                        format options:8                        bits for different options
//        ?color            U32                                        (may add this later)
typedef struct ParseTable
{
    char const* name;
    StructTypeField type;
    size_t storeoffset;
    intptr_t param; // default to ints, but pointers must fit here
    void* subtable;
    StructFormatField format;
} ParseTable;
// terminate table with an empty name field

// Legacy spelling must name the same struct, including in forward declarations.
#define TokenizerParseInfo ParseTable

#endif // TEXTPARSER_TYPES_H
