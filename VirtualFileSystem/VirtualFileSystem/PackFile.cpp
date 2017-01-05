#include "PackFile.h"
#include <algorithm>

PackFile::PackFile(std::unique_ptr<File> pak_file, size_t start_index, size_t file_size):m_PackFile(std::move(pak_file)),m_StartIndex(start_index),m_FileSize(m_FileSize),m_CurrentByteIndex(0)
{
}

PackFile::~PackFile()
{
}

bool PackFile::Open(FileMode mode)
{
	if(mode==FileMode::ReadMode)
	{
		if(!m_PackFile->Open(FileMode::ReadMode))
		{
			return  false;
		}
		m_PackFile->SetByteIndex(m_StartIndex);
		m_CurrentByteIndex = 0;
	}
	return false;
}

size_t PackFile::Read(uint8_t* buffer, size_t size)
{
	size_t real_byte_count;
	size_t maximum_byte_count = (m_FileSize - m_CurrentByteIndex);
	
	real_byte_count = std::min(maximum_byte_count, size);
	auto read_byte_count = m_PackFile->Read(buffer, real_byte_count);
	m_CurrentByteIndex += read_byte_count;
	return read_byte_count;
}

size_t PackFile::Write(const uint8_t* buffer, size_t size)
{
	throw "Writing to pack file is not allowed";
	return size_t();
}

void PackFile::Close()
{
	m_CurrentByteIndex = -1;
	m_PackFile->Close();
}

size_t PackFile::GetByteIndex()
{
	return size_t();
}

void PackFile::SetByteIndex(const size_t index)
{
	m_PackFile->SetByteIndex(index + m_StartIndex);
}
