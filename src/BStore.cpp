//
// BStore.cpp
// BlindStorage
//

#include "BStore.h"

BStore::BStore(){
	totalFileBlocks = 0;
}

BStore::~BStore(){
//	delete[] allFileBytes;
}

BStore::BStore(string directoryPath): D(TOTAL_BLOCKS){
	totalFileBlocks = 0;
	vector<string> filesList;
	readFileNamesFromDirectory(directoryPath, filesList);
//	allFileBytes = new byte[totalFileBlocks*BLOCK_SIZE]();

	cout << "Processing files..." << endl;
	b_index_t numBlocksRead = 0;
	for(int i = 0; i < filesList.size(); i++){
		string filepath = filesList[i];
		
		cout << ((double)(i+1)*100)/(double)filesList.size() << "\% i.e. " << i+1 << "/" << filesList.size() << " processed";
		cout.flush();
		cout << "\r";
		
		size_t size = readFileSize(filepath);
		byte fileBytes[size];

		b_index_t numBlocks = (b_index_t)ceil((double)size/(double)MAX_BLOCK_DATA_SIZE);
		
//		byte fileBytes[numBlocks*BLOCK_SIZE];
		readFile(filepath, fileBytes, size);

		cout << "Writing " << filesList[i] << endl;
		fileID fid(filesList[i]);
		PRSubset prSubset(numBlocks*BLOW_UP);

		T.addFile(fid, prSubset);
		D.addFile(fileBytes, size, fid, prSubset);
		numBlocksRead += numBlocks;
	}
	cout << endl;
	
	cout << "Finalizing T..." << endl;
	T.finalize(D);
	cout << "Finalizing D..." << endl;
	D.encryptEmptyBlocks();

//	cout << "Preprocessing took " << ((double)(clock() - begin))/CLOCKS_PER_SEC << endl;

	cout << "Writing D to disk." << endl;
	D.writeToDisk();
	cout << "Writing T to disk." << endl;
	T.writeToDisk();
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
