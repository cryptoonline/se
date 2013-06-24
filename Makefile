CC=g++
RM=rm

CFLAGS=-c -g -O0
LDFLAGS=-lcryptopp -lboost_system-mt -lboost_filesystem-mt -L/usr/local/Cellar/boost/1.53.0/lib -I/usr/local/Cellar/boost/1.53.0/include -I /Users/naveed/BStore/test/framework/gtest-1.6.0/include/ /Users/naveed/BStore/test/*.cc /Users/naveed/BStore/test/framework/gtest_build/libgtest.a
SOURCES=AES.cpp BStore.cpp CRIBlock.cpp CRI.cpp DataBlock.cpp Ddisk.cpp TBlock.cpp Tdisk.cpp HashMAC.cpp PRF.cpp PRSubset.cpp fileID.cpp TruncAES.cpp Key.cpp OnlineSession.cpp Debug.cpp helper.cpp 
# Blowfish.cpp Communicator.cpp Cri.cpp DataBlock.cpp DataStructures.cpp Ddisk.cpp Debug.cpp PRF.cpp OnlineSession.cpp PRSubset.cpp SHA256bit.cpp SSE.cpp TBlock.cpp Tdisk.cpp Tfile.cpp fileID.cpp BStore.cpp Key.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=blindStorage

SOURCE_DIR=src/
OBJECT_DIR=obj/
BIN_DIR=bin/

CSOURCES=$(addprefix $(SOURCE_DIR),$(SOURCES))
COBJECTS=$(addprefix $(OBJECT_DIR),$(OBJECTS))
CEXECUTABLE=$(addprefix $(BIN_DIR),$(EXECUTABLE))

all: $(CSOURCES) $(CEXECUTABLE)

$(CEXECUTABLE): $(COBJECTS)
	$(CC) $(LDFLAGS) $(COBJECTS) -o $@

$(OBJECT_DIR)%.o: $(SOURCE_DIR)%.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) $(COBJECTS)

#g++ ./../src/AES.cpp ./../src/Debug.cpp -I /Users/naveed/BStore/test/framework/gtest-1.6.0/include/ /Users/naveed/BStore/test/*.cc /Users/naveed/BStore/test/framework/gtest_build/libgtest.a -I /Users/naveed/BStore/src -lcryptopp -lboost_system-mt -lboost_filesystem-mt -L/usr/local/Cellar/boost/1.53.0/lib -I/usr/local/Cellar/boost/1.53.0/include -o AES_test
