// 
// Searchable Encryption
// FileStore.cpp
//

#include "FileStore.h"

FileStore::FileStore(){
}

FileStore::~FileStore(){
}

size_t FileStore::get(string filename, byte contents[]){
	std::ifstream in(filename.c_str(), std::ios::binary);
	size_t size = readFileSize(filename.c_str());
	in.read(reinterpret_cast<char*>(contents), size);
	in.close();
	return size;
}

void FileStore::put(string filename, byte contents[], size_t size){
	ofstream out(filename.c_str(), std::ios::binary);
	out.write(reinterpret_cast<char*>(contents), size);
	out.close();
}

void FileStore::copy(string srcPath, string dstPath){
	std::ifstream src(srcPath.c_str(), std::ios::binary);
	std::ofstream dst(dstPath.c_str(), std::ios::binary);

	dst << src.rdbuf();

	src.close();
	dst.close();
}

void FileStore::remove(string path){
	if(std::remove(path.c_str()) != 0)
		cout << "Error deleting file " << path << endl;
	else
		cout << "File successfully delete." << endl;
}

bool FileStore::isFilePresent(string path){
	if( access(path.c_str(), F_OK) != -1 )
		return true;
	else
		return false;
}

size_t FileStore::readFileSize(string path){
	struct stat st;
	stat(path.c_str(), &st);
	return st.st_size;
}
