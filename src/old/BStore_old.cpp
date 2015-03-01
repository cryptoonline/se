//
// BStore.cpp
// BlindStorage
//

#include "BStore.h"

BStore::BStore(Communicator &communicator){
	this->communicator = communicator;
}

BStore::BStore(unordered_map< string, unordered_set<uint64_t> >& map) : D(TOTAL_BLOCKS){
	clock_t begin;
	clock_t end;
	double time = 0;
	for(unordered_map<string, unordered_set<docid_t> >::iterator itmap = map.begin(); itmap != map.end(); ++itmap) {
		const string & keyword = itmap->first;
		unordered_set<docid_t> & set = itmap->second;
		unsigned char documents[set.size()*sizeof(docid_t)];
		int counter = 0;
		
		for(unordered_set<docid_t>::iterator itset = set.begin(); itset != set.end(); ++itset) {
			docid_t documentId = *itset;
			memcpy(&documents[counter*sizeof(docid_t)], static_cast<unsigned char*>(static_cast<void*>(&documentId)), sizeof(docid_t)); 
			counter++;
		}
//		if(keyword == "corner"){
//			cout << "Set size is " << set.size() << endl;
//			getchar();
//		}
		int32_t documentsListSize = set.size()*sizeof(docid_t);

		begin = clock();
		int32_t numBlocks = (int32_t)ceil((double)documentsListSize/BLOCK_SIZE) * BLOW_UP;
		PRSubset prSubset(numBlocks);
		T.addFile(keyword, prSubset);
		D.addFile(keyword, documents, documentsListSize, prSubset);
		end = clock();
		time += ((double)(end-begin))/CLOCKS_PER_SEC;
	}
	
	begin = clock();
	T.finalize(D);
	end = clock();
		time += ((double)(end-begin))/CLOCKS_PER_SEC;
	cout << "Writing T to disk" << endl;
	T.writeToDisk();

	begin = clock();
	D.finalize();
	end = clock();
		time += ((double)(end-begin))/CLOCKS_PER_SEC;
	cout << "Writing D to disk" << endl;
	D.writeToDisk();

	cout << "Preprocessing took " << time << " seconds." << endl;
}

BStore::BStore(Communicator &communicator, string directoryPath): D(TOTAL_BLOCKS){
	this->communicator = communicator;
	readFilesFromDirectory(directoryPath);
	cout << "Processing files" << endl;
	for(int i = 0; i < filesList.size(); i++){
		//cout << __PRETTY_FUNCTION__ << "Processing file " << filesList[i] << endl;
		cout << ((double)i*100)/(double)filesList.size() << "\% i.e. " << i << "/" << filesList.size() << " processed";
		cout.flush();
		cout << "\r";
		int32_t fileSize = readFileSize(filesList[i]);
		uint32_t numBlocks =(uint32_t) ceil((double)fileSize/MAX_BLOCK_DATA_SIZE) * BLOW_UP;
		PRSubset prSubset(numBlocks);
		T.addFile(filesList[i], prSubset);
		D.addFile(filesList[i], prSubset);	
	}
	cout << endl;
	T.finalize(D);
	cout << "Writing T to disk" << endl;
	
	D.finalize();
	//cout << "Preprocessing took " << ((double)(clock() - begin))/CLOCKS_PER_SEC << endl;
	cout << "Writing D to disk" << endl;
	D.writeToDisk();

	T.writeToDisk();



}

BStore::~BStore(){
}

void BStore::upload(){
}

bool BStore::read(string filename, vector<unsigned char>& fileContents){
	OnlineSession session(communicator);
	bool fileStatus = session.get(filename, 0, fileContents); //Second arugment is size and is needed for write and update
	print_docid_t(fileContents, "BSTORE READ");
	return fileStatus;
}

void BStore::write(string filename, unsigned char* filedata, uint32_t filesize){
	OnlineSession session(communicator);
	vector<unsigned char> fileContents;
	cout << "File size is " << filesize;
	session.get(filename, filesize, fileContents);
	session.update(filedata, filesize, filename);
}

unsigned char* BStore::update(string filename, unsigned char* updatedFiledata, uint32_t filesize){
}

void BStore::del(string filename){
}

void BStore::rename(string filename){
}

// void BStore::readFilesFromDirectory(string directory){
// 	/* Code for this function is taken from: http://stackoverflow.com/questions/306533/how-do-i-get-a-list-of-files-in-a-directory-in-c*/
//  	DIR *dir;
//      class dirent *ent;
//      class stat st;
//  	
//      dir = opendir(directory.c_str());
//  
//      while ((ent = readdir(dir)) != NULL) {
//      	const string file_name = ent->d_name;
//      	string full_file_name = directory + '/' + file_name;
//  		
//      	if (file_name[0] == '.')
//      		continue;
//  
//      	if (stat(full_file_name.c_str(), &st) == -1)
//      		continue;
//  		
//  		
//  		const bool is_directory = (st.st_mode & S_IFDIR) != 0;
//  	
//  		if(!is_directory){
//  			filesList.push_back(full_file_name);
//  			cout << "Processing file " << full_file_name << endl;
//  		}
//  			
//      	if (is_directory){
//  			full_file_name = directory + file_name;
//  			readFilesFromDirectory(full_file_name);
// 		}
//     }
//     closedir(dir);
// }

void BStore::readFilesFromDirectory(string path){
	for(boost::filesystem::recursive_directory_iterator end, dir(path); dir!=end; ++dir){
		string fileName = dir->path().string();
		if(boost::filesystem::is_regular(dir->status())){
			cout << "Processing " << fileName << endl;
			filesList.push_back(fileName);
		}
	}
}


ifstream::pos_type BStore::readFileSize(string filename){
	ifstream file(filename.c_str(), std::ios_base::in | std::ios_base::binary | std::ios_base::ate);
	return file.tellg();
}
