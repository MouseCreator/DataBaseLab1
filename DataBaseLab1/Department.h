#pragma once

#include <iostream>


#define NAME_LENGTH 25
#define WEBSITE_LENGTH 50

struct Department {
	int firstTeacherNumber = -1;
	bool isDeleted = false;
	unsigned short index = -1;
	char name[NAME_LENGTH] = "_NN";
	char website[WEBSITE_LENGTH] = "_NW";

	void print() {
		std::cout << "DEPARTMENT:" << "\n\tIndex: " << index <<
			"\n\tName: " << name << "\n\tWebsite: " << website << "\n\tTeacher: " << firstTeacherNumber << std::endl;
	}
};
