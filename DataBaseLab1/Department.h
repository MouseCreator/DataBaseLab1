#pragma once

#include <iostream>


#define NAME_LENGTH 25
#define WEBSITE_LENGTH 50

struct Department {
	int firstTeacherNumber;
	bool isDeleted = false;
	unsigned short index;
	char name[NAME_LENGTH];
	char website[WEBSITE_LENGTH];
};
