//
// BStore.cpp
// BlindStorage
//

#include "BStore.h"

BStore::BStore(Communicator &communicator, string directoryPath): D(TOTAL_BLOCKS){
	this->communicator = communicator;
	readFilesFromDirectory(directoryPath);
	for(int i = 0; i < filesList.size(); i++){
		//cout << __PRETTY_FUNCTION__ << "Processing file " << filesList[i] << endl;
		if(i % 1000 == 0)
			cout << (i*100)/filesList.size() << "\% i.e. " << i << "/" << filesList.size() << " processed" << endl;
		D.addFile(filesList[i]);
	}
	cout << "Writing files to disk";
	D.writeToDisk();
}

BStore::~BStore(){
}

void BStore::upload(){
}

unsigned char* BStore::read(string filename){
//	OnlineSession session;
//	return session.get(filename);
}

void BStore::write(string filename, unsigned char* filedata){
}

unsigned char* BStore::update(string filename, unsigned char* updatedFiledata){
}

void BStore::del(string filename){
}

void BStore::rename(string filename){
}

void BStore::readFilesFromDirectory(string directory){
	/* Code for this function is taken from: http://stackoverflow.com/questions/306533/how-do-i-get-a-list-of-files-in-a-directory-in-c*/
	DIR *dir;
    class dirent *ent;
    class stat st;
	
    dir = opendir(directory.c_str());

    while ((ent = readdir(dir)) != NULL) {
    	const string file_name = ent->d_name;
    	const string full_file_name = directory + "/" + file_name;
		
		//cout << "Processing file " << full_file_name << endl;
		
    	if (file_name[0] == '.')
    		continue;

    	if (stat(full_file_name.c_str(), &st) == -1)
    		continue;

		const bool is_directory = (st.st_mode & S_IFDIR) != 0;

    	if (is_directory){
			readFilesFromDirectory(full_file_name);
		}
    	
		filesList.push_back(full_file_name);
    }
    closedir(dir);
}
