// 
// Searchable Encryption
// FileStore.cpp
//

#include "FileStore.h"

FileStore::FileStore(){
	if(isFilePresent(FILESTORE_PATH)){
		std::cerr << "Error: Filestore doesn't exist." << endl;
		exit(1);
	}
}

FileStore::~FileStore(){
}

size_t FileStore::get(string filename, byte*& contents){
//	filename = FILESTORE_PATH + filename;
	std::ifstream in(filename.c_str(), std::ios::binary);
	size_t size = readFileSize(filename.c_str());
	contents = new byte[size]; /* Function calling this method should manage this memory*/
	in.read(reinterpret_cast<char*>(contents), size);
	in.close();
	return size;
}

void FileStore::put(string filename, byte contents[], size_t size){
	filename = FILESTORE_PATH + filename;
	ofstream out(filename.c_str(), std::ios::binary);
	out.write(reinterpret_cast<char*>(contents), size);
	out.close();
}

void FileStore::copy(string srcPath, string dstPath){
	srcPath = FILESTORE_PATH + srcPath;
	dstPath = FILESTORE_PATH + dstPath;

	std::ifstream src(srcPath.c_str(), std::ios::binary);
	std::ofstream dst(dstPath.c_str(), std::ios::binary);

	dst << src.rdbuf();

	src.close();
	dst.close();
}

void FileStore::remove(string filename){
	filename = FILESTORE_PATH + filename;

	if(std::remove(filename.c_str()) != 0)
		cout << "Error deleting file " << filename << endl;
	else
		cout << "File successfully delete." << endl;
}

bool FileStore::isFilePresent(string filename){
	filename = FILESTORE_PATH + filename;
	if( access(filename.c_str(), F_OK) != -1 )
		return true;
	else
		return false;
}
