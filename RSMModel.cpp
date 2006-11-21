#include "RSMModel.h"
#include "filesystem.h"
extern cFileSystem fs;


void cRSMModel::load(string filename)
{
	cFile* pFile = fs.open(filename);

	char buffer[100];
	pFile->read(buffer, 6); // header
	pFile->read(buffer, 26); // unknown

	pFile->read(buffer, 4); // ntextures;

	long ntextures = *((long*)buffer);



	pFile->close();



}

void cRSMModel::draw()
{

}