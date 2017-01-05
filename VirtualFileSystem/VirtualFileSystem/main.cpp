#include "FileSystem.h"
#include "iostream"
#include "string"
#include <cassert>

int main(int args, char *argv[] )
{
	FileSystem system;
	system.MountDirectory(std::string("C:\\Users\\Student\\Desktop\\Test"));
	system.LogAllFiles();
	std::cin.get();

	auto fullpath = system.GetPhysicalFilePath("input.txt");

	std::cout << fullpath << std::endl;
	std::cin.get();



	//system.MountArchive(std::string("test.zip"));
	std::vector<std::string>files;
	system.GetFilesInDirectory(files,"C:\\Users\\Student\\Desktop\\Test");
	for (auto & path : files)
	{
		std::cout << path << std::endl;
	}
	std::cin.get();
	files.clear();
	system.GetFilesWithExtension(files, "jpg");
	for (auto & path : files)
	{
		std::cout << path << std::endl;
	}
	std::cin.get();

	std::unique_ptr<File> input_file = system.GetFile("input.txt");
	std::unique_ptr<File> output_file = system.GetOutputFile("output.txt");

	input_file->Open(FileMode::ReadMode);
	output_file->Open(FileMode::WriteMode);

	size_t read_byte_count, written_byte_count;
	read_byte_count = 0;
	std::vector<uint8_t> buffer;

	constexpr int buffer_size = 1024;

	buffer.resize(buffer_size);

	do
	{
		read_byte_count = input_file->Read(buffer.data(), buffer.size());
		if (read_byte_count > 0 && read_byte_count <= buffer_size - 1)
			buffer[read_byte_count] = '\0';
		buffer.resize(read_byte_count+1);
		written_byte_count = output_file->Write(buffer.data(), buffer.size());

		assert(written_byte_count == read_byte_count);

	} while (read_byte_count > 0);

	input_file->Close();
	output_file->Close();
	std::cin.get();

}
