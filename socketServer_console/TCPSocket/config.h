#ifndef __CONFIG__H_
#define __CONFIG__H_

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#define Sleep(a) usleep(a*1000)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#endif // __CONFIG__H_
