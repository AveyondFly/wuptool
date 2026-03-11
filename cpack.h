/*
  cpack - Pack Wii U NUS content files (encryption support)

  Copyright (C) 2024

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
*/

#ifndef _CPACK_H
#define _CPACK_H

#include <stdint.h>
#include <stdbool.h>
#include "aes.h"

// Hash block sizes
#define HASH_BLOCK_SIZE     0xFC00
#define HASH_BLOCK_TOTAL    0x10000
#define HASHES_SIZE         0x0400
#define HASH_SIZE           20

// Content types
#define CONTENT_TYPE_ENCRYPTED  0x2001
#define CONTENT_TYPE_HASHED     0x2003

// Title types
#define TITLE_TYPE_BASE     0x00050000
#define TITLE_TYPE_UPDATE   0x0005000E
#define TITLE_TYPE_DLC      0x0005000C

// Max entries in FST
#define MAX_PACK_ENTRIES    90000

// FST Entry structure for packing
typedef struct {
    char*       path;           // Full file path
    char*       name;           // File name only
    uint32_t    name_offset;    // Offset in string table
    uint32_t    entry_offset;   // Entry number
    uint32_t    file_offset;    // Offset in content (for files)
    uint32_t    file_size;      // File size
    uint16_t    content_id;     // Content ID this file belongs to
    uint16_t    flags;          // FST flags
    bool        is_dir;         // Is directory
    bool        is_root;        // Is root entry
    int         parent_idx;     // Parent entry index
    int         next_idx;       // Next entry (for dirs)
    int         child_count;    // Number of children
    int*        children;       // Children indices
} pack_fst_entry;

// Content info for packing
typedef struct {
    uint32_t    id;             // Content ID (e.g., 0x00000004)
    uint16_t    index;          // Content index
    uint16_t    type;           // Content type
    uint32_t    size;           // Encrypted size
    uint32_t    decrypted_size; // Decrypted data size
    bool        hashed;         // Is hashed content
    uint8_t     hash[20];       // SHA1 hash for TMD
    char        filename[32];   // Output filename
    uint32_t    file_count;     // Number of files in this content
    uint32_t    data_offset;    // Current write offset
} pack_content_info;

// Title metadata for packing
typedef struct {
    uint64_t    title_id;
    uint32_t    version;
    uint16_t    title_type;     // 0x00=base, 0x0E=update, 0x0C=DLC
    uint32_t    group_id;
    char        common_key[16]; // Encryption key
    char        title_key[16];  // Decrypted title key
    uint8_t     encrypted_key[16]; // Encrypted title key (from original ticket)
    bool        has_encrypted_key; // Whether encrypted_key is available
    
    pack_content_info* contents;
    uint32_t    content_count;
    
    pack_fst_entry* entries;
    uint32_t    entry_count;
    uint32_t    string_table_size;
} pack_title_info;

// Hash tree structure
typedef struct {
    uint8_t*    h0;             // H0 hashes (block_count * 20 bytes)
    uint8_t*    h1;             // H1 hashes
    uint8_t*    h2;             // H2 hashes
    uint8_t*    h3;             // H3 hashes
    uint32_t    block_count;
} hash_tree;

// Function declarations

// Hash tree operations
hash_tree* hash_tree_create(uint32_t block_count);
void hash_tree_free(hash_tree* ht);
int hash_tree_calculate(hash_tree* ht, const uint8_t* data, uint32_t data_size);
int hash_tree_get_hashes_for_block(hash_tree* ht, uint32_t block, uint8_t* out_hashes);

// Content encryption
int encrypt_content(const char* input_dir, const char* output_dir,
                    pack_title_info* info, pack_content_info* content,
                    const uint8_t* title_key, hash_tree* ht);

int encrypt_content_hashed(const char* input_dir, const char* output_dir,
                           pack_title_info* info, pack_content_info* content,
                           const uint8_t* title_key);

int encrypt_content_simple(const char* input_dir, const char* output_dir,
                           pack_title_info* info, pack_content_info* content,
                           const uint8_t* title_key);

// FST building
int build_fst_from_metadata(const char* meta_path, pack_title_info* info);
uint8_t* build_fst_data(pack_title_info* info, uint32_t* out_size);

// TMD/Ticket generation
int generate_tmd(const pack_title_info* info, const char* output_path);
int generate_ticket(const pack_title_info* info, const char* output_path);
int generate_cert(const char* output_path);

// Metadata parsing
int parse_metadata_json(const char* path, pack_title_info* info);

// Main pack function
int pack_title(const char* input_dir, const char* output_dir, const char* title_key_hex, const char* common_key_hex);

#endif // _CPACK_H
