#include "Windows.h"
#include "FileSystem.h"
#include "algorithm"
#include <iostream>
#include "PhysicalFile.h"
#include  "File.h"
#include "FileHeader.h"
#include "PackFile.h"
#include <iterator>


FileSystem::FileSystem()
{
}


FileSystem::~FileSystem()
{
}


void FileSystem::MountDirectory(const std::string& directory)
{
	HANDLE hFind;
	WIN32_FIND_DATAA data;
	hFind = FindFirstFileA(((directory+"\\*").c_str()), &data);
	
	if(hFind==INVALID_HANDLE_VALUE)
	{
		printf("FindFirstFile failed (%d)\n", GetLastError());
		return;
	}
	else
	{
		do
		{
			if(data.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			{
				if(strcmp(data.cFileName,".")==0
					|| strcmp(data.cFileName,"..")==0)
				{
					continue;
				}
				MountDirectory((directory + "\\" + data.cFileName));
			}
			else
			{
				//printf("%s\n", data.cFileName);
				m_Files.push_back(
				{
					directory,
					data.cFileName,	
				});
			}

		} while (FindNextFileA(hFind, &data)!=0);
	}

	FindClose(hFind);
}

void FileSystem::MountArchive(const std::string& archive)
{
	auto file = GetFile(archive);
	file->Open(FileMode::ReadMode);

	do
	{	
		ZipFileHeader file_header;
		file->Read(reinterpret_cast<uint8_t*>(&file_header), sizeof(ZipFileHeader));
		if (file_header.signature != 0x4034b50)
		{
			break;
		}		

		std::vector<char> filename;
		filename.reserve(file_header.filename_length + 1);
		file->Read(reinterpret_cast<uint8_t*>(filename.data()), file_header.filename_length);
		filename[file_header.filename_length] = 0;

		file->Read(nullptr, file_header.extra_field_length);
		FileEntry file_entry;
		file_entry.m_Name = std::string(filename.data());
		file_entry.m_Size = file_header.uncompressed_size;
		file_entry.m_StartIndex = file->GetByteIndex();
		file_entry.m_Pack = archive.data();

		file->Read(nullptr, file_header.compressed_size);

	} while (true);

	file->Close();
}

std::string FileSystem::GetPhysicalFilePath( const std::string& filename) const
{	
	auto it = std::find_if(m_Files.begin(), m_Files.end(), [filename](const FileEntry & file)
	{
		if (file.m_Name == filename)
		{
			return true;
		}
	});
	if(it!=m_Files.end())
	{
		FileEntry temp = static_cast<FileEntry>(*it);
		return std::string(temp.m_Directory + "\\" + temp.m_Name);
	}

	return  std::string();
}

void FileSystem::GetFilesInDirectory(std::vector<std::string>& file_table, const std::string& directory) 
{
	HANDLE hFind;
	WIN32_FIND_DATAA data;
	hFind = FindFirstFileA(((directory + "\\*").c_str()), &data);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf("FindFirstFile failed (%d)\n", GetLastError());
		return;
	}
	else
	{
		do
		{
			if (data.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			{
				if (strcmp(data.cFileName, ".") == 0
					|| strcmp(data.cFileName, "..") == 0)
				{
					continue;
				}
				GetFilesInDirectory(file_table,(directory + "\\" + data.cFileName));
			}
			else
			{
				int counter = 0;
				counter++;

				
				//printf("%s\n", data.cFileName);
				m_Files.push_back({
					directory,
					data.cFileName
				});

				file_table.push_back(data.cFileName);
				
			}

		} while (FindNextFileA(hFind, &data) != 0);
	}

	FindClose(hFind);
}

void FileSystem::GetFilesWithExtension(std::vector<std::string>& file_table, const std::string& extension) const
{
	
	if (m_Files.empty())
		return;

	for (FileEntry file : m_Files)
	{
		std::string temp = file.m_Name;

		std::reverse(temp.begin(), temp.end());
		

		int counter = 0;
		std::find_if(temp.begin(), temp.end(), [&](char v){	
			counter++;
			return v == '.';
		});
		temp = temp.substr(0, counter - 1);
		std::reverse(temp.begin(), temp.end());

		if(temp==extension)
		{
			file_table.push_back(file.m_Name);
		}

	}
	std::cout << "File extension: " << extension << std::endl;
}

std::unique_ptr<File> FileSystem::GetFile(const std::string& filename) const
{

	auto * file_entry = GetFileEntry(filename);

	if(file_entry==nullptr)
	{
		return std::unique_ptr<File>();
	}
	if(file_entry->m_Pack !="")
	{
		return std::unique_ptr<File>(
			new PackFile(
				GetFile(file_entry->m_Pack),
				file_entry->m_StartIndex,
				file_entry->m_Size));
	}
	else
	{
		return std::unique_ptr<File>(
			new PhysicalFile(
				file_entry->m_Directory +"\\"+file_entry->m_Name));
	}
}

std::unique_ptr<File> FileSystem::GetOutputFile(const std::string& filename) const
{
	auto * file_entry = GetFileEntry(filename);

	if (file_entry == nullptr)
	{
		return std::unique_ptr<File>();
	}
	if (file_entry->m_Pack != "")
	{
		return std::unique_ptr<File>(
			new PackFile(
				GetFile(file_entry->m_Pack),
				file_entry->m_StartIndex,
				file_entry->m_Size));
	}
	else
	{
		return std::unique_ptr<File>(
			new PhysicalFile(
				file_entry->m_Directory +"\\" + file_entry->m_Name));
	}
}

void FileSystem::LogAllFiles()
{
	std::for_each(m_Files.begin(),m_Files.end(),[](const FileEntry & file)
	{
		std::cout << file.m_Directory <<"\\"<<file.m_Name<< std::endl;
	});
}

const FileSystem::FileEntry* FileSystem::GetFileEntry(const std::string& filename) const
{
	auto it = std::find_if(
		m_Files.begin(),
			m_Files.end(),
				[=](const FileEntry & entry)
	{
		return filename == entry.m_Name;
	});

	if(it!=m_Files.end())
	{
		return &*it;
	}

	return nullptr;
}
