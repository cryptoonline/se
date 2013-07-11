#include <Python/Python.h>
#include <string>
using std::string;

#include <iostream>

typedef unsigned char byte;

int Dropbox_Interface(char*, char*, char*, char*, size_t);

void upload(string filename, char* file, size_t size){
	char filenameChars[filename.size()];
	memcpy(filenameChars, filename.c_str(), filename.size());
	Dropbox_Interface("Dropbox", "upload", filenameChars, file, size);
}

void download(string filename, char* file){
	char filenameChars[filename.size()];
	memcpy(filenameChars, filename.c_str(), filename.size());
	Dropbox_Interface("Dropbox", "download", filenameChars, file, 0);	
}

int Dropbox_Interface(char* pythonfile,  char* function, char* filename, char* fileContents, size_t size)
{
    PyObject *pName, *pModule, *pDict, *pFunc;
    PyObject *pArgs, *pValue;
    int i;

    Py_Initialize();
    pName = PyString_FromString(pythonfile);
    /* Error checking of pName left out */

//		PySys_SetPath("."); 

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, function);
        /* pFunc is a new reference */
				int numArgs = 2;
        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(numArgs);
            for (i = 0; i < numArgs; ++i) {
			  			if(i == 0)
								pValue = PyString_FromString(filename);
							else if (i == 1)
								pValue = PyString_FromString(fileContents);
	//						else if (i == 2)
		//						pValue = PyInt_FromLong(size);
              if (!pValue) {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    fprintf(stderr, "Cannot convert argument\n");
                    return 1;
                }
                /* pValue reference stolen here: */
                PyTuple_SetItem(pArgs, i, pValue);
            }
            pValue = PyObject_CallObject(pFunc, pArgs);
			if(function == "download"){
				PyObject* objectsRepresentation = PyObject_Repr(pValue);
				const char* downloadedFile = PyString_AsString(objectsRepresentation);
				memcpy(fileContents, downloadedFile, sizeof(downloadedFile));
				printf("%s", downloadedFile);
			}
            Py_DECREF(pArgs);
            if (pValue != NULL) {
                printf("Result of call: %ld\n", PyInt_AsLong(pValue));
                Py_DECREF(pValue);
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return 1;
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", function);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", pythonfile);
        return 1;
    }
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
