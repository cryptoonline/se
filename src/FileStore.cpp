// 
// Searchable Encryption
// FileStore.cpp
//

#include "FileStore.h"

FileStore::FileStore(){
}

FileStore::~FileStore(){
}

void FileStore::get(string srcPath, string dstPath){
	std::ifstream src(srcPath.c_str(), std::ios::binary);
	std::ofstream dst(dstPath.c_str(), std::ios::binary);

	dst << src.rdbuf();

	src.close();
	dst.close();
}

void FileStore::put(string srcPath, string dstPath){
	std::ifstream src(srcPath.c_str(), std::ios::binary);
	std::ofstream dst(dstPath.c_str(), std::ios::binary);

	dst << src.rdbuf();

	src.close();
	dst.close();
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
