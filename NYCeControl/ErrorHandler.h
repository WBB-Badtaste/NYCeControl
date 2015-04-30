#pragma once

#include <nyceapi.h>
#include <fstream>
#include <iostream>
#include <Windows.h>

#include "Configure.h"

using namespace std;

class ErrorHandler
{
private:
	ofstream *pLogFile;

public:
	ErrorHandler(void)
	{
		SYSTEMTIME time;
		CHAR path[100];
		GetSystemTime(&time);
		sprintf_s(path, "%s%s_%d-%d-%d.txt", ERROR_LOG_ADDR, ERROR_LOG_NAME, time.wYear, time.wMonth, time.wDay);
		pLogFile = new ofstream(path, ios::out | ios::app);
	}

	~ErrorHandler()
	{
		pLogFile->close();
		delete pLogFile;
	}

	void HandleError(NYCE_STATUS status, const char *name)
	{
		SYSTEMTIME time;
		GetSystemTime(&time);
		*pLogFile<<"time: "<<time.wHour<<":"<<time.wMinute<<":"<<time.wSecond<<"    "<<"error occur at: "<<name<<"    "<<"error: "<<NyceGetStatusString(status)<<"\n"<<endl;
	}

};

