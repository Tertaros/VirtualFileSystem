#pragma once
#include <cstdint>
#include "Mode.h"

class File
{
public:



	File();
	virtual ~File();

	virtual size_t GetByteIndex() = 0;
	virtual void SetByteIndex(const size_t index) = 0;

	virtual bool Open(FileMode mode) = 0;
	virtual size_t Read(uint8_t * buffer, size_t size) = 0;
	virtual size_t Write(const uint8_t * buffer, size_t size) = 0;
	virtual void Close() = 0;

};

