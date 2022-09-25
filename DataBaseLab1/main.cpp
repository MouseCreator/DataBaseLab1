#include <iostream>

#include "Department.h"
#include "DepartmentIndex.h"
#include "GarbageList.h"

const char* GARBAGE_FILE = "garbage.fl";
const char* DEPARTMENT_INDEX_FILE = "department.index";

int addToFl(Department department);
int getNumberOfDepartmentIndex(FILE* fp);
void addToIndex(DepartmentIndex* index);

int menu();

DepartmentIndex* createDepartmentIndex(int key, int address) {
	DepartmentIndex* index = new DepartmentIndex();
	index->key = key;
	index->address = address;
	return index;
}

bool checkIndex(int index) {

	FILE* fp;
	fopen_s(&fp, DEPARTMENT_INDEX_FILE, "rb+");
	if (fp) {

		//find number of indexes in file
		int size = getNumberOfDepartmentIndex(fp);
		DepartmentIndex department;
		bool result = false;
		//read all departments and place new department in place to keep it sorted
		for (int i = 0; i < size; i++) {
			fread(&department, sizeof(DepartmentIndex), 1, fp);
			if (department.key == index) {
				puts("This index already exists!\n");
				result = true;
				break;
			}

		}
		fclose(fp);
		return result;
	}
	return true;
}

Department createDepartment() {

	Department department;
	int index = 0;
	do {
		printf("Index: ");
		scanf_s("%d", &index);
	} while (checkIndex(index));

	department.index = index;

	printf("Name: ");
	fgets(department.name, NAME_LENGTH, stdin);
	department.name[strcspn(department.name, "\n")] = 0;


	printf("WebSite: ");
	fgets(department.website, WEBSITE_LENGTH, stdin);
	department.website[strcspn(department.website, "\n")] = 0;


	department.index = 4;

	return department;
}

int getUnusedIndex() {
	FILE* fp;
	fopen_s(&fp, GARBAGE_FILE, "rb");

	if (!fp) {
		printf("Can't find garbage file!");
		return -1;
	}

	GarbageList* list = NULL;
	while (!feof(fp)) {
		GarbageList* next = new GarbageList();
		fread(&next->address, sizeof(int), 1, fp);
		next->next = list;
		list = next;
	}
	fclose(fp);
	if (list == NULL) {
		return -1; //no garbage
	}
	//get first garbage value
	int result = list->address;
	GarbageList* first = list;

	//write the rest of the garbage
	list = list->next;
	fopen_s(&fp, GARBAGE_FILE, "wb");
	while (list) {
		fwrite(&list->address, sizeof(int), 1, fp);
		list = list->next;
	}
	//free memory
	list = first;
	while (list) {
		GarbageList* toDelete = list;
		free(toDelete);
		list = list->next;
	}
	fclose(fp);
	return result;

}

void addGarbage(int address) {
	FILE* fp;
	fopen_s(&fp, GARBAGE_FILE, "a+");
	if (!fp) {
		printf("Can't find garbage file!");
		return;
	}
	fwrite(&address, sizeof(address), 1, fp);
	fclose(fp);
}

void addToFiles(Department department) {
	int addedTo = addToFl(department);
	if (addedTo != -1) {
		DepartmentIndex* index = createDepartmentIndex(department.index, addedTo);
		addToIndex(index);
		free(index);
	}
}

int addToFl(Department department) {
	const char CINEMA_FL_FILE[] = "department.fl";
	FILE* fp;
	fopen_s(&fp, CINEMA_FL_FILE, "rb+");
	int position = -1;
	if (fp) {
		int offset = getUnusedIndex();
		if (offset == -1) { //no gardage
			fseek(fp, 0, SEEK_END);
		}
		else { //overwrite garbage
			fseek(fp, sizeof(department) * offset, SEEK_SET);
		}
		fwrite(&department, sizeof(department), 1, fp);
		position = ftell(fp) / sizeof(department);
		fclose(fp);
	}
	else {
		printf("Error opening %s", CINEMA_FL_FILE);
	}
	
	return position;
}


int getNumberOfDepartmentIndex(FILE* fp) {
	fseek(fp, 0, SEEK_END);
	int len = ftell(fp);
	int size = len / sizeof(DepartmentIndex);
	rewind(fp);
	return size;
}

void addToIndex(DepartmentIndex* index) {

	FILE* fp;
	fopen_s(&fp, DEPARTMENT_INDEX_FILE, "rb+");
	if (fp) {

		//find number of indexes in file
		int size = getNumberOfDepartmentIndex(fp);
		size++;

		DepartmentIndex* departments = new DepartmentIndex[size];

		bool foundPlace = false;

		//read all departments and place new department in place to keep it sorted
		for (int i = 0; i < size; i++) {
			fread(&departments[i], sizeof(DepartmentIndex), 1, fp);
			if (!foundPlace && departments[i].key > index->key) {
				foundPlace = true;
				std::swap(departments[i], departments[i + 1]);
				departments[i].address = index->address;
				departments[i].key = index->key;
				i++;
			}
		}
		fclose(fp);

		//rewrite departments index file
		fopen_s(&fp, DEPARTMENT_INDEX_FILE, "wb");
		if (fp) {
			fwrite(departments, sizeof(departments), 1, fp);
			fclose(fp);
		}
		delete[] departments;
	}
	else {
		printf("Error opening %s", DEPARTMENT_INDEX_FILE);
	}
}

int main(void) {
	return menu();
}


void insertDepartment() {
	Department d = createDepartment();
	addToFiles(d);
}

int menu() {
	char command[10];
	while (true) {
		printf("Command: ");
		fgets(command, 10, stdin);
		command[strcspn(command, "\n")] = 0;


		if (strcmp(command, "insert_m") == 0) {
			insertDepartment();
		}
		else if (strcmp(command, "insert_s") == 0) {

		}
		else if (strcmp(command, "get_m") == 0) {

		}
		else if (strcmp(command, "get_s") == 0) {

		}
		else if (strcmp(command, "del_m") == 0) {

		}
		else if (strcmp(command, "del_s") == 0) {

		}
		else if (strcmp(command, "update_m") == 0) {

		}
		else if (strcmp(command, "update_s") == 0) {

		}
		else if (strcmp(command, "help") == 0) {

		}
		else if (strcmp(command, "exit") == 0) {
			break;
		}
		else {
			puts("Unknown command\n");
		}
	}
	return 0;
}