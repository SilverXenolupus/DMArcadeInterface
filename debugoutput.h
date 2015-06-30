#ifndef _DEBUGOUTPUT_H_
#define _DEBUGOUTPUT_H_

#include <iostream>
#include <fstream>
using namespace std;

class DebugOutput
{
public : 
	void OutputVideoInfo();
	char* VideocardName;
	int Memory;

private:
};

#endif