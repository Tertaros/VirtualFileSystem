#pragma once
#include "File.h"
#include <string>

class PhysicalFile_Linux:public File
{
public:
	PhysicalFile_Linux(const std::string & file_path);
	~PhysicalFile_Linux();

	virtual bool Open(FileMode mode) override;
	virtual size_t Read(uint8_t * buffer, size_t size) override;
	virtual size_t Write(const uint8_t * buffer, size_t size) override;
	virtual void Close() override;
	virtual size_t GetByteIndex() override;
	virtual void SetByteIndex(const size_t index)override;
private:
	//HANDLE m_Handle;
	int m_FileDescriptor;
	std::string m_Path;
};

