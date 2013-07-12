//
// DropboxAPI.cpp
// BlindStorage
//

#include "DropboxAPI.h"

DropboxAPI::DropboxAPI() : pythonfile("Dropbox"), pythonClass("DropboxAPI"){
	int i;
	Py_Initialize();
 	pName = PyString_FromString(pythonfile.c_str());
	pModule = PyImport_Import(pName);
	pDict = PyModule_GetDict(pModule);
	pClass = PyDict_GetItemString(pDict, pythonClass.c_str());

	if(PyCallable_Check(pClass))
		pInstance = PyObject_CallObject(pClass, NULL);
}

DropboxAPI::~DropboxAPI(){
	Py_DECREF(pModule);
	Py_DECREF(pName);
	Py_Finalize();
}

void DropboxAPI::upload(string filename, char* file, size_t size){
	char function[] = "upload";
	
	char filenameChars[filename.size()];
	memcpy(filenameChars, filename.c_str(), filename.size());
	
	pValue = PyObject_CallMethod(pInstance, function, "(ss)", filenameChars, file);

	if(pValue != NULL){
		printf("Return of call : %d\n", PyInt_AsLong(pValue));
		Py_DECREF(pValue);
	}
	else{
		PyErr_Print();
	}
}

size_t DropboxAPI::download(string filename, char*& file){
	char function[] = "download";

	char filenameChars[filename.size()];
	memcpy(filenameChars, filename.c_str(), filename.size());
	
	pValue = PyObject_CallMethod(pInstance, function, "(ss)", filenameChars, file);

	if(pValue != NULL){
		PyObject* objectsRepresentation = PyObject_Repr(pValue);
		const char* downloadedFile = PyString_AsString(objectsRepresentation);
		string downloadedFileString(downloadedFile);
		file = new char[downloadedFileString.size()];
		memcpy(file, downloadedFile, downloadedFileString.size());
		return downloadedFileString.size();
	
		printf("Return of call : %d\n", PyInt_AsLong(pValue));
		Py_DECREF(pValue);
	}
	else{
		PyErr_Print();
		return 0;
	}
}

int main(){
	char file[1024];
	memset(file, 'n', 1023);
	file[1023] = '\0';
	DropboxAPI dbapi;
	dbapi.upload("finalTesting2", file, 1024);
	dbapi.upload("finalTesting3", file, 1024);

	DropboxAPI dbapi1;
	char* downloadfile;
	size_t downloadsize = dbapi1.download("finalTesting2", downloadfile);
	printf("%s\n", downloadfile);
	cout << "Size of the download file is " << downloadsize << endl;
	delete[] downloadfile;
	return 0;
}
