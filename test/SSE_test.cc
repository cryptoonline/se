//
// SSE_test.cc
// Searchable Encryption Test
//

#include "gtest/gtest.h"

#include <stdio.h>

#include "./../src/parameters.h"
#include "./../src/SSE.h"

void deletefile(string file);

TEST(SSEReadTest, Test1){
	/* Delete D and T created by BlindStorage Tests */
//	deletefile(D_FILE);
//	deletefile(T_FILE);

	SSE sse;
//	string directoryPath = "/Users/naveed/BStore/datasets/email/enron_mail_20110402/maildir/mann-k/inbox/";

	string directoryPath = "/Users/naveed/BStore/datasets/testdir/";
	sse.indexgen(directoryPath);

	while(1){
		string keyword;
		cout << "Enter keyword to search for (Enter q to quit): ";
		cin >> keyword;
		if(keyword.compare("q") == 0)
			break;

		vector<docid_t> docIDs;
		if(sse.search(keyword, docIDs)){
			cout << "*************************************************docIDs***************************************************" << endl; 
			for(int i = 0; i < docIDs.size(); i++)
				cout << docIDs[i] << "\t";
			cout << endl;
			cout << "*************************************************docIDs***************************************************" << endl; 
//			printdec(docIDs.data(), docIDs.size(), "Documents Retrieved containing word " + keyword);
		}
		else
			cout << "No files with keyword " << keyword << " found." << endl;
	}
}

void deletefile(string file)
{
  if( remove(file.c_str()) != 0 )
    perror( "Error deleting file" );
  else
    puts( "File successfully deleted" );
}
