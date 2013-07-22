//
// OnlineSession_test.cc
// BlindStorage Test
//

#include "gtest/gtest.h"

#include "./../../src/OnlineSession.h"
#include "./../../src/BStore.h"
#include "./../../src/DiskCommunicator.h"

TEST(OnlineSessionGet, Test1){
//	string directoryPath = "/Users/naveed/BStore/datasets/email/enron_mail_20110402/maildir/mann-k/inbox/";
	string directoryPath = "/Users/naveed/BStore/datasets/testdir";
	BStore store(directoryPath);
	DiskCommunicator dcomm;
	OnlineSession session;
	
//	std::string filename = "/Users/naveed/BStore/datasets/email/enron_mail_20110402/maildir/mann-k/inbox/260.";
	string filename = "/Users/naveed/BStore/datasets/testdir/test";
	size_t size = 2787;
	byte* contents = new byte[size];
	session.read(filename, contents);

	printchars(contents, size, "File contents");

}
