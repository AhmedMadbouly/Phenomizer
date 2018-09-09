#ifndef _QF_EXCEPTION_H_
#define _QF_EXCEPTION_H_

#include <string>
#include <stdexcept>

using namespace std;

class QFException : public logic_error
{	
public:
	QFException(string msg) : logic_error(msg)
	{
	}
};

class FileErrorException : public runtime_error
{
	string fileName;
public:
	FileErrorException(string msg = "",string fileName = "") : runtime_error(msg)
	{
		this->fileName = fileName;
	}

	string FileName() const {return fileName;}
};

class FileNotFoundException : public FileErrorException
{
public:
	FileNotFoundException(string fileName) : FileErrorException("File " + fileName + " is not found!", fileName)
	{
	}
};


#endif
