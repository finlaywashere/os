#pragma once
#include <stdint.h>
#include <kernel/fs/echfs.h>
#define NO_ARCH 0
#define SPACE 2
#define X86 3
#define MIPS 8
#define POWERPC 0x14
#define ARM 0x28
#define SUPERH 0x2A
#define IA-64 0x32
#define X86-64 0x3E
#define AARCH646 0xB7

struct elf_header{
	char magic[4];
	uint8_t bits;
	uint8_t endian;
	uint8_t elfHeaderVersion;
	uint8_t abi;
	char reserved[8];
	uint16_t executable;
	uint16_t instructionSet;
	uint32_t elfVersion;
	uint64_t entryPosition;
	uint64_t headerTablePosition;
	uint64_t sectionTablePosition;
	uint32_t flags;
	uint16_t headerSize;
	uint16_t headerEntrySize;
	uint16_t numHeaderEntries;
	uint16_t sectionSize;
	uint16_t sectionEntrySize;
	uint16_t numSectionEntries;
	uint16_t headerNameIndex;
}__attribute__((packed));
typedef struct elf_header elf_header_t;

#define SEGMENT_NULL 0
#define SEGMENT_LOAD 1
#define SEGMENT_DYNAMIC 2
#define SEGMENT_NOTE 4

#define FLAG_EXECUTABLE 1
#define FLAG_WRITABLE 2
#define FLAG_READABLE 4

struct elf_program_header{
	uint32_t segmentType;
	uint32_t flags;
	uint64_t dataOffset;
	uint64_t memOffset;
	uint64_t undefined;
	uint64_t sizeInFile;
	uint64_t sizeInMemory;
	uint64_t align;
}__attribute__((packed));
typedef struct elf_program_header elf_program_header_t;

struct loaded_elf{
	uint64_t text_ptr;
	uint64_t text_vptr;
	uint64_t text_len;
	uint64_t data_ptr;
	uint64_t data_vptr;
	uint64_t data_len;
	uint64_t entry_point;
};
typedef struct loaded_elf loaded_elf_t;

loaded_elf_t* load_elf(directory_entry_t* file);
