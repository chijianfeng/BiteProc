#pragma once

#ifndef _THREAD_UTIL_
#define _THREAD_UTIL_
#include <afx.h>

//save the eeg data to the file.
DWORD WINAPI SaveFileThread(LPVOID lparam);

//load the eeg data from the file 
DWORD WINAPI LoadFileThread(LPVOID lparam);

//process and anlysis the data
DWORD WINAPI ProcessThread(LPVOID lparam);

//in train mode thread 
DWORD WINAPI TrainThread(LPVOID lparam);

DWORD WINAPI OLProcessThread(LPVOID lparam);			//process the data on line base the trained model.

#endif
 