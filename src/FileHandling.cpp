//
// FileHandling.cpp
// BlindStorage
//

#include "FileHandling.h"

void readFileNamesFromDirectory(string path, vector<string>& filesList){
	for(boost::filesystem::recursive_directory_iterator end, dir(path); dir!=end; ++dir){
		string fileName = dir->path().string();
		if(boost::filesystem::is_regular(dir->status())){
			cout << "Processing " << fileName << endl;
			if(fileName.compare("/Users/naveed/BStore/datasets/testdir/.DS_Store") == 0)
				continue;
			filesList.push_back(fileName);
			totalFileBlocks += (b_index_t)ceil((double)readFileSize(fileName)/(double)MAX_BLOCK_DATA_SIZE);
		}
	}
}

size_t readFileSize(string path){
	struct stat st;
	stat(path.c_str(), &st);
	return st.st_size;
}

void readFile(string path, byte fileBytes[], size_t size){
	ifstream file(path.c_str());
	file.seekg(0, std::ios::beg);
	file.read(reinterpret_cast<char*>(fileBytes), size);
	file.close();
}
