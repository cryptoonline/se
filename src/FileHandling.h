//
// FileHandling.h
// BlindStorage
//

#ifndef __BlindStorage__FileHandling__
#define __BlindStorage__FileHandling__
	
void readFileNamesFromDirectory(string path, vector<string>& filesList);
void readFile(string path, byte fileBytes[], size_t size);
size_t readFileSize(string path);

#endif /* defined(__BlindStorage__FileHandling__) */
