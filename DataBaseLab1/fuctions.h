#pragma once
#include <iostream>

#include "Department.h"
#include "DepartmentIndex.h"
#include "GarbageList.h"
#include "Teacher.h"
//user interface
int menu();

//department
DepartmentIndex* createDepartmentIndex(int key, int address);
bool checkIndex(int index);
void changeDepartmentFileds(Department& department, bool updateMode = false);
Department createDepartment();
int getUnusedIndex(const char* file);
void addGarbage(int address, const char* file);
void addToFiles(Department department);
int addToFl(Department department);
int getNumberOfDepartmentIndex(FILE* fp);
void addToIndex(DepartmentIndex* index);
int binarySearch(DepartmentIndex* arr, int left, int right, int key);
DepartmentIndex getDepartmentIndex(int key);
Department findByAddress(int address);
Department getDepartment(int key);
void printDepartment();
void updateDepartmentByKey();
void removeFromIndex(int key);
void deleteDepartment(int key);
void deleteDepartmentByKey();
void updateDepartmentFile(int address, Department department);
void printAllDepartments();
void insertDepartment();

//teacher
void insertTeacher();
void changeTeacherFields(Teacher& teacher, bool updateMode = false);
Teacher createTeacher(int firstTeacherAddress);
Teacher getTeacher(int key, int firstTeacherAddress);
bool hasTeacher(int key, int firstTeacherAddress);
Teacher readTeacher(FILE* fp, int position);
void writeTeacher(FILE* fp, Teacher teacher, int position);
int writeTeacher(FILE* fp, Teacher teacher);
int insertToFile(Teacher teacher, int firstPosition);
void printAllTeachers();
void printTeacher();
void directPrintTeachers();