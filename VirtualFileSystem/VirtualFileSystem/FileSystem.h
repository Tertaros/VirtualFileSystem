#pragma once
#include "string"
#include "vector"
#include "File.h"
#include <memory>
class FileSystem
{
public:
	FileSystem();
	~FileSystem();

	

	void MountDirectory(const std::string & directory);
	void MountArchive(const std::string & archive);
	std::string GetPhysicalFilePath( const std::string & filename)const;
	void GetFilesInDirectory(
		std::vector<std::string> & file_table, const std::string & directory);
	void GetFilesWithExtension(
		std::vector<std::string> & file_table, const std::string & extension) const;
	std::unique_ptr<File>GetFile(const std::string & filename) const;
	std::unique_ptr<File>GetOutputFile(const std::string & filename) const;
	void LogAllFiles();
private:

struct FileEntry
{

	std::string m_Directory;
	std::string m_Name;
	std::string m_Pack;
	std::size_t m_Size;
	std::size_t m_StartIndex;


	bool operator==(const FileSystem::FileEntry & rhs)
	{
		if (this->m_Name == rhs.m_Name)
			return true;
		else
			return false;
	}
	FileEntry(){}

	FileEntry(std::string directory, std::string name):m_Directory(directory),m_Name(name)
	{
		
	}
};

public:
	const FileEntry * GetFileEntry(const std::string & filename) const; 

	std::vector<FileEntry>  m_Files;


};

