#pragma once


#include <iostream>
#include <stdlib.h>
#include <unistd.h>

using namespace std;


#define INFO_LOG(fmt, args...) fprintf(stdout, "[INFO]  " fmt "\n", ##args);
#define WARN_LOG(fmt, args...) fprintf(stdout, "[WARN]  " fmt "\n", ##args);
#define ERROR_LOG(fmt, args...) fprintf(stdout, "[ERROR]  " fmt "\n", ##args);


typedef enum Result {
	SUCCESS = 0,
	FAILED = 1
} Result;



