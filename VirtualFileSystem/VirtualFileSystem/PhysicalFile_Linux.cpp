#include "PhysicalFile_Linux.h"
#ifdef __linux__
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>



PhysicalFile_Linux::PhysicalFile_Linux(const std::string & file_path)
{
}


PhysicalFile_Linux::~PhysicalFile_Linux()
{
}

bool PhysicalFile_Linux::Open(FileMode mode)
{
	switch (mode)
	{
	case FileMode::ReadMode:
		m_FileDescriptor = open(m_Path, O_RDONLY);
		if (m_FileDescriptor == -1)
		{
			perror("Open");
			return 2;
		}
		break;
	case FileMode::WriteMode:
		m_FileDescriptor=open(m_Path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (m_FileDescriptor == -1) {
			perror("open");
			return 3;
		}
	case FileMode::ReadWriteMode:
		m_FileDescriptor = open(m_Path, O_RDWR | O_CREAT | O_TRUNC, 0644);
		if (m_FileDescriptor == -1) {
			perror("open");
			return 4;
		}
		return m_FileDescriptor != -1;
	}
}

size_t PhysicalFile_Linux::Read(uint8_t* buffer, size_t size)
{
	m_FileDescriptor = read(m_FileDescriptor, buffer,size);

	//if(m_FileDescriptor==-1)
	//{
	//	perror("Open");
	//	return 2;
	//}
	return m_FileDescriptor;
}

size_t PhysicalFile_Linux::Write(const uint8_t* buffer, size_t size)
{
	m_FileDescriptor = write(m_FileDescriptor,buffer,size);

	return m_FileDescriptor;
}

void PhysicalFile_Linux::Close()
{
	close(m_FileDescriptor);
}

size_t PhysicalFile_Linux::GetByteIndex()
{
}

void PhysicalFile_Linux::SetByteIndex(const size_t index)
{
}
#endif