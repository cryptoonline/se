CC=g++
RM=rm

CFLAGS=-c -Wall -ggdb
LDFLAGS=-lcryptopp 
SOURCES=AES.cpp Blowfish.cpp Communicator.cpp DataBlock.cpp Ddisk.cpp Debug.cpp PRF.cpp OnlineSession.cpp PRSubset.cpp SHA256bit.cpp TBlock.cpp Tdisk.cpp Tfile.cpp fileID.cpp BStore.cpp Key.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main

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
