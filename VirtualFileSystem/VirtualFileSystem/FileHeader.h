#pragma once
#include <cstdint>

#pragma pack(push,1) //Does not work in Linux, search there for a way too keep the alignment in a structure
struct ZipFileHeader
{
	int32_t signature;
	int16_t version;
	int16_t flags;
	int16_t compression;
	int16_t filetime;
	int16_t filedate;
	int32_t crc;
	int32_t compressed_size;
	int32_t uncompressed_size;
	int16_t filename_length;
	int16_t extra_field_length;
};
#pragma  pack(pop)