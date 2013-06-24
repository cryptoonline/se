//
// BStore.cpp
// BlindStorage
//

#include "BStore.h"

BStore::BStore(){
}

BStore::BStore(string directoryPath): D(TOTAL_BLOCKS){
	vector<string> filesList;
	readFileNamesFromDirectory(directoryPath, filesList);
	
	cout << "Processing files..." << endl;
	for(int i = 0; i < filesList.size(); i++){
		string filepath = filesList[i];
		
		cout << ((double)i*100)/(double)filesList.size() << "\% i.e. " << i << "/" << filesList.size() << " processed";
		cout.flush();
		cout << "\r";
		
		size_t size = readFileSize(filepath);
		
		b_index_t numBlocks = (b_index_t)ceil((double)size/(double)MAX_BLOCK_DATA_SIZE);
		
		byte fileBytes[numBlocks*BLOCK_SIZE];
		readFile(filepath, fileBytes, numBlocks);

		fileID fid(filesList[i]);
		PRSubset prSubset(numBlocks*BLOW_UP);

		T.addFile(fid, prSubset);
		D.addFile(fileBytes, size, fid, prSubset);
	}
	cout << endl;
	
	T.finalize(D);
	D.encryptEmptyBlocks();
//	cout << "Preprocessing took " << ((double)(clock() - begin))/CLOCKS_PER_SEC << endl;
//	cout << "Writing D to disk" << endl;
//	D.writeToDisk();
//	T.writeToDisk();



}

BStore::~BStore(){
}

void BStore::readFileNamesFromDirectory(string path, vector<string>& filesList){
	for(boost::filesystem::recursive_directory_iterator end, dir(path); dir!=end; ++dir){
		string fileName = dir->path().string();
		if(boost::filesystem::is_regular(dir->status())){
			cout << "Processing " << fileName << endl;
			filesList.push_back(fileName);
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

void BStore::readFile(string filename, byte contents[], b_index_t numBlocks){
	ifstream file(filename.c_str());
	file.seekg(0, std::ios::beg);
	for(b_index_t i = 0; i < numBlocks; i++)
		file.read(reinterpret_cast<char*>(&contents[i*BLOCK_SIZE]), MAX_BLOCK_DATA_SIZE);
	file.close();
}
