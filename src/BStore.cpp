//
// BStore.cpp
// BlindStorage
//

#include "BStore.h"

BStore::BStore():D(TOTAL_BLOCKS){
	totalFileBlocks = 0;
}

BStore::~BStore(){
}

void BStore::add(string filename, byte fileBytes[], size_t size){
	b_index_t numBlocks = (b_index_t)ceil((double)size/(double)MAX_BLOCK_DATA_SIZE);
	fileID fid(filename);
	PRSubset prSubset(numBlocks*BLOW_UP, filename);
	D.addFile(fileBytes, size, fid, prSubset); 
}

void BStore::finalize(double& execTime){
	clock_t startTime = clock();
	cout << "Total number of blocks occupied are " << D.getNumOccupiedBlocks() << endl;
	D.encryptEmptyBlocks();
	
	execTime += (double)(clock()-startTime)/(double)CLOCKS_PER_SEC;

	clock_t writeStartTime = clock();
	D.writeToDisk();
	cout << "Disk Writing took " << ((double)(clock()-writeStartTime)/(double)CLOCKS_PER_SEC) << " seconds." << endl;
}

void BStore::readFileNamesFromDirectory(string path, vector<string>& filesList){
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

//	ifstream::pos_type BStore::readFileSize(string filename){
//		ifstream file(filename.c_str(), std::ios_base::in | std::ios_base::binary | std::ios_base::ate);
//	return file.tellg();
//	}

size_t BStore::readFileSize(string path){
	struct stat st;
	stat(path.c_str(), &st);
	return st.st_size;
}

void BStore::readFile(string path, byte fileBytes[], size_t size){
	ifstream file(path.c_str());
	file.seekg(0, std::ios::beg);
	file.read(reinterpret_cast<char*>(fileBytes), size);
//	for(b_index_t i = 0; i < numBlocks; i++)
//		file.read(reinterpret_cast<char*>(&contents[i*BLOCK_SIZE]), MAX_BLOCK_DATA_SIZE);
	file.close();
}
