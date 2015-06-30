#include "debugoutput.h"

void DebugOutput::OutputVideoInfo()
{
	ofstream videofile;
	videofile.open("./VideoLog.txt", ios::out | ios::app);
	videofile << "Video Card Name : " << VideocardName << "\n" << "Video Card Memory : " << Memory << "\n";
	videofile.close();
}