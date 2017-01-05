#pragma once
#include "File.h"
#include <memory>

class PackFile:public File
{
public:
	PackFile(
		std::unique_ptr<File> pak_file,
		size_t start_index,
		size_t file_size);
	~PackFile();

	virtual bool Open(FileMode mode) override;
	virtual size_t Read(uint8_t * buffer, size_t size) override;
	virtual size_t Write(const uint8_t * buffer, size_t size) override;
	virtual void Close() override;
	virtual size_t GetByteIndex() override;
	virtual void SetByteIndex(const size_t index)override;

private:
	std::unique_ptr<File>m_PackFile;
	size_t m_StartIndex;
	size_t m_FileSize;
	size_t m_CurrentByteIndex;
};
