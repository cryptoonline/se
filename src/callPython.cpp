#include <Python/Python.h>
#include <string>
using std::string;

#include <iostream>

typedef unsigned char byte;

int Dropbox_Interface(char*, char*, char*, size_t);

void upload(string filename, char* file, size_t size){
	char filenameChars[filename.size()];
	memcpy(filenameChars, filename.c_str(), filename.size());
	Dropbox_Interface("upload", filenameChars, file, size);
}

void download(string filename, char* file){
	char filenameChars[filename.size()];
	memcpy(filenameChars, filename.c_str(), filename.size());
	Dropbox_Interface("download", filenameChars, file, 0);	
}

int Dropbox_Interface(char* function, char* filename, char* fileContents, size_t size){
	char pythonfile[] = "Dropbox";
	char pythonClass[] = "DropboxAPI";
		
	PyObject *pName, *pModule, *pDict, *pFunc;
	PyObject *pClass, *pArgs, *pValue;
	PyObject *pInstance;
	int i;

	Py_Initialize();
	pName = PyString_FromString(pythonfile);
	/* Error checking of pName left out */

	pModule = PyImport_Import(pName);

	//pDict is borrowed reference
	pDict = PyModule_GetDict(pModule);
	
	//Build the name of a callable class
	pClass = PyDict_GetItemString(pDict, pythonClass);

	//Create an instance of the class
	if(PyCallable_Check(pClass))
		pInstance = PyObject_CallObject(pClass, NULL);

	pValue = PyObject_CallMethod(pInstance, function, "(ss)", filename, fileContents);

	if(function == "download"){
		PyObject* objectsRepresentation = PyObject_Repr(pValue);
		const char* downloadedFile = PyString_AsString(objectsRepresentation);
		memcpy(fileContents, downloadedFile, sizeof(downloadedFile));
		printf("%s", downloadedFile);
	}
	
	if(pValue != NULL){
		printf("Return of call : %d\n", PyInt_AsLong(pValue));
		Py_DECREF(pValue);
	}
	else{
		PyErr_Print();
	}

	Py_DECREF(pModule);
	Py_DECREF(pName);
	Py_Finalize();
	return 0;
}

int main(){
	char file[1024];
	memset(file, 'a', 1024);
	upload("testing12345", file, 1024);
	download("testing12345", file);
	printf("%s\n", file);
	return 0;
}
