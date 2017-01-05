#pragma once
#include "File.h"
#include <string>
#include "Windows.h"

class PhysicalFile: public File
{
public:
	PhysicalFile(const std::string & file_path);
	virtual ~PhysicalFile();

	virtual bool Open(FileMode mode) override;
	virtual size_t Read(uint8_t * buffer, size_t size) override;
	virtual size_t Write(const uint8_t * buffer, size_t size) override;
	virtual void Close() override;
	virtual size_t GetByteIndex() override;
	virtual void SetByteIndex(const size_t index)override;
private :
	HANDLE m_Handle;
	std::string m_Path;

};

