#include "PhysicalFile.h"

#include <handleapi.h>
#include <fileapi.h>


PhysicalFile::PhysicalFile(const std::string & file_path):m_Path(file_path)
{
}


PhysicalFile::~PhysicalFile()
{
}

bool PhysicalFile::Open(FileMode mode)
{
	DWORD access;
	DWORD creation_disposition = 0;
	switch (mode)
	{
	case FileMode::ReadMode:
		access = GENERIC_READ;
		creation_disposition = OPEN_ALWAYS;
		break;
	case FileMode::WriteMode:
		access = GENERIC_WRITE;
		creation_disposition = OPEN_EXISTING;//CREATE_NEW;
		break;
	case FileMode::ReadWriteMode:
		access = GENERIC_WRITE || GENERIC_READ;
		creation_disposition = CREATE_NEW;
		break;
	default:
		throw "Unknown mode";
	}

	m_Handle = CreateFileA(
		m_Path.c_str(),
		access,
		0,
		NULL,
		creation_disposition,
		FILE_ATTRIBUTE_NORMAL,
		NULL);


	return m_Handle != INVALID_HANDLE_VALUE;
}

size_t PhysicalFile::Read(uint8_t* buffer, size_t size)
{
	//LPOVERLAPPED ol = { 0 };

	//LPDWORD sizeofFilesRead = 0;
	//if(false==ReadFile(
	//			m_Handle,
	//			buffer,
	//			size,
	//			nullptr,
	//			nullptr))
	//{
	//	printf("Terminal failure: Unable to read from file ");
	//	CloseHandle(m_Handle);
	//	return size_t();
	//}
	//return size_t();
	DWORD byte_read=0;
	ReadFile(m_Handle, buffer, size-1, &byte_read, nullptr);
	return byte_read;
	
}

size_t PhysicalFile::Write(const uint8_t* buffer, size_t size)
{
	DWORD byte_written=0;
	WriteFile(m_Handle, buffer, size, &byte_written, nullptr);
	return byte_written;
}

void PhysicalFile::Close()
{
	if(m_Handle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_Handle);
		m_Handle = INVALID_HANDLE_VALUE;
	}
}

size_t PhysicalFile::GetByteIndex()
{
	return size_t();
}

void PhysicalFile::SetByteIndex(const size_t index)
{
}
