// 
// DropboxAPI.h
// BlindStorage
//

#ifndef __BlindStorage__Dropbox__
#define __BlindStorage__Dropbox__

#include <Python/Python.h>

#include<string>
using std::string;

#include <iostream>
using std::cout;
using std::endl;

#include <stdio.h>
#include <string.h>

typedef unsigned char byte;

class DropboxAPI {
private:
	string pythonfile;
	string pythonClass;

	PyObject *pName, *pModule, *pDict, *pFunc;
	PyObject *pClass, *pArgs, *pValue, *pInstance;

public:
	DropboxAPI();
	~DropboxAPI();
	void upload(string filenames, char* file, size_t filesize);
	void download(string filenames, uint32_t numFiles, char* file, size_t filesize);
};

#endif /* defined(__BlindStorage__Dropbox__) */
