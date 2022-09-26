#pragma once
#include <iostream>
#define TEACHER_NAME_LENGTH 100
#define EMAIL_LENGTH 50
#define DEGREE_LENGTH 50
#define SCIENTIFIC_TITLE_LENGTH 50
struct Teacher {
	int tabnumber = -1;
	int next = -1;
	bool isDeleted = false;
	char name[TEACHER_NAME_LENGTH] = "_NM";
	char email[EMAIL_LENGTH] = "_NE";
	char degree[DEGREE_LENGTH] = "_ND";
	char scintific_title[SCIENTIFIC_TITLE_LENGTH] = "_NT";
	void print() {
		std::cout << "TEACHER:" << std::endl <<
			"\tTabnumber: " << tabnumber << std::endl <<
			"\tName: " << name << std::endl <<
			"\tE-mail: " << email << std::endl <<
			"\tDegree: " << degree << std::endl <<
			"\tTitle: " << scintific_title << std::endl;

	}
};
