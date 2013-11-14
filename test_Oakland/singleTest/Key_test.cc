//
// Key_test.cc
// BlindStorage Test
//

#include <cstring>

#include "gtest/gtest.h"
#include "./../../src/Key.h"
#include "./../../src/Debug.h"

using std::cout;

TEST(KeyCreation, KeySize16) {
	int keySize = 16;
	Key key("KeyTest16Bytes", keySize);
	std::ifstream in("KeyTest16Bytes");
	EXPECT_TRUE(in.good());
}

TEST(KeyCreation, KeySize20) {
	int keySize = 20;
	Key key("KeyTest20Bytes", keySize);
	std::ifstream in("KeyTest20Bytes");
	EXPECT_TRUE(in.good());
}

TEST(KeyAccess, KeySize16) {
	int keySize = 16;
	Key key("KeyTest16Bytes", keySize);
	byte testKey[keySize];
	key.get(testKey);
	printhex(testKey, keySize, "16 Byte Key Disk Access Test");	
	cout << "Assuming that randomly generated key will never be all zeros." << endl;
	byte zeroVector[keySize];
	memset(zeroVector, 0, keySize);

	EXPECT_FALSE( 0 == std::memcmp(testKey, zeroVector, keySize));
}

TEST(KeyAccess, KeySize20) {
	int keySize = 20;
	Key key("KeyTest20Bytes", keySize);
	byte testKey[keySize];
	key.get(testKey);
	printhex(testKey, keySize, "20 Byte Key Disk Access Test");
	cout << "Assuming that randomly generated key will never be all zeros." << endl;
	byte zeroVector[keySize];
	memset(zeroVector, 0, keySize);

	EXPECT_FALSE( 0 == std::memcmp(testKey, zeroVector, keySize));
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
