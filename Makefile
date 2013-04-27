CC=g++
RM=rm

CFLAGS=-c -Wall -g
LDFLAGS=-lcryptopp -Lsrc/cryptopp562
SOURCES=AES.cpp Communicator.cpp DataBlock.cpp Ddisk.cpp FFX.cpp OnlineSession.cpp PRSubset.cpp SHA256bit.cpp TBlock.cpp Tdisk.cpp Tfile.cpp fileID.cpp main.cpp
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
