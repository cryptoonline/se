//
// main.cpp
// BlindStorage
//

#include "AES.h"
#include "OnlineSession.h"
#include "BStore.h"
#include "DiskCommunicator.h"
#include "helper.h"
#include "Debug.h"

int main(){

	string directoryPath = "datasets/email/enron_mail_20110402/maildir/mann-k/inbox/";
	BStore* store = new BStore();

	int numFiles = 12;
	string filenumbers[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9" , "10", "267", "207"};
	for(int i = 0; i < numFiles; i++){
		string filename = directoryPath + filenumbers[i] + ".";
		size_t size = readFileSize(filename);
		byte fileBytes[size];
		readFile(filename, fileBytes, size);

		printhex(fileBytes, size, "FILE BYTES");

		store->add(filename, fileBytes, size);
	}
	double execTime;
	store->finalize(execTime);
	delete store;
	
	return 0;
}
