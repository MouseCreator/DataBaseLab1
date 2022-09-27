
#include "fuctions.h"

const char* GARBAGE_FILE = "garbage.fl";
const char* DEPARTMENT_INDEX_FILE = "department.index";
const char* DEPARTMENT_FL_FILE = "department.fl";



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

void changeDepartmentFileds(Department& department, bool updateMode) {
	Department prev = department;
	std::cout << "Name: ";
	std::cin >> department.name;
	if (updateMode && strcmp(department.name, "") == 0) {
		strcpy_s(department.name, prev.name);
	}

	std::cout << "Website: ";
	std::cin >> department.website;
	if (updateMode && strcmp(department.website, "") == 0) {
		strcpy_s(department.website, prev.website);
	}
}

Department createDepartment() {

	Department department;
	do {
		std::cout << "Index: ";
		std::cin >> department.index;
	} while (checkIndex(department.index) || department.index == -1);

	changeDepartmentFileds(department);

	return department;
}

int getUnusedIndex(const char* file) {
	FILE* fp;
	fopen_s(&fp, file, "rb+");

	if (!fp) {
		puts("Can't find garbage file!");
		return -1;
	}

	GarbageList* list = NULL;
	while (true) {
		int address;
		fread(&address, sizeof(int), 1, fp);
		if (feof(fp)) {
			break;
		}
		GarbageList* next = new GarbageList();
		next->address = address;
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
		list = list->next;
		delete toDelete;
		
	}
	fclose(fp);
	return result;

}

void addGarbage(int address, const char* file) {
	FILE* fp;
	fopen_s(&fp, file, "a+");
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
		delete index;
	}
}

int addToFl(Department department) {
	FILE* fp;
	fopen_s(&fp, DEPARTMENT_FL_FILE, "rb+");
	int position = -1;
	if (fp) {
		int offset = getUnusedIndex(GARBAGE_FILE);
		if (offset == -1) { //no gardage
			fseek(fp, 0, SEEK_END);
		}
		else { //overwrite garbage
			fseek(fp, sizeof(department) * offset, SEEK_SET);
		}
		position = ftell(fp) / sizeof(department); //where to put
		fwrite(&department, sizeof(department), 1, fp);
		fclose(fp);
	}
	else {
		printf("Error opening %s", DEPARTMENT_FL_FILE);
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
		for (int i = 0; i < size - 1; i++) {
			DepartmentIndex toAdd;
			fread(&toAdd, sizeof(toAdd), 1, fp);
			if (!foundPlace && toAdd.key > index->key) {
				foundPlace = true;
				departments[i].address = index->address;
				departments[i].key = index->key;
			}
			departments[i+foundPlace].address = toAdd.address;
			departments[i+foundPlace].key = toAdd.key;
		}
		if (!foundPlace) {
			departments[size-1].address = index->address;
			departments[size-1].key = index->key;
		}
		fclose(fp);

		//rewrite departments index file
		fopen_s(&fp, DEPARTMENT_INDEX_FILE, "wb");
		if (fp) {
			fwrite(departments, sizeof(departments), size, fp);
			fclose(fp);
		}
		delete[] departments;
	}
	else {
		printf("Error opening %s", DEPARTMENT_INDEX_FILE);
	}
}

int binarySearch(DepartmentIndex* arr, int left, int right, int key) {
	if (right >= left) {
		int mid = left + (right - left) / 2;

		if (arr[mid].key == key) {
			return mid;
		}

		return arr[mid].key > key ? binarySearch(arr, left, mid - 1, key) : binarySearch(arr, mid + 1, right, key);
	}
	return -1;
}

DepartmentIndex getDepartmentIndex(int key) {
	FILE* fp;
	DepartmentIndex result;
	fopen_s(&fp, DEPARTMENT_INDEX_FILE, "r+");
	if (fp) {
		int size = getNumberOfDepartmentIndex(fp);
		DepartmentIndex* departments = new DepartmentIndex[size];
		fread(departments, sizeof(DepartmentIndex), size, fp);
		fclose(fp);
		int index = binarySearch(departments, 0, size - 1, key);
		if (index == -1) {
			std::cout << "No such key in the index file!" << std::endl;
		}
		else {
			result = departments[index];
		}
		delete[] departments;
	}
	else {
		std::cout << "Can't open" << DEPARTMENT_INDEX_FILE << std::endl;
	}
	return result;
}

Department findByAddress(int address) {
	FILE* fp;
	fopen_s(&fp, DEPARTMENT_FL_FILE, "r+");
	Department result;
	if (!fp) {
		std::cout << "Can't open" << DEPARTMENT_INDEX_FILE << std::endl;
	}
	else {
		fseek(fp, sizeof(Department) * address, SEEK_SET);
		fread(&result, sizeof(Department), 1, fp);
		fclose(fp);
	}
	return result;
}

Department getDepartment(int key) {
	DepartmentIndex index = getDepartmentIndex(key);
	return findByAddress(index.address);
}

void printDepartment() {
	int key;
	std::cout << "Key: ";
	std::cin >> key;
	Department department = getDepartment(key);
	if (department.index != -1) {
		department.print();
	}
	else {
		std::cout << "Can't find this department." << std::endl;
	}
}

void updateDepartment(int key) {
	Department updated = getDepartment(key);
	std::cout << "Type updated info:\n";
	changeDepartmentFileds(updated, true);
	DepartmentIndex index = getDepartmentIndex(key);
	updateDepartmentFile(index.address, updated);
}
void updateDepartmentByKey() {
	int key;
	std::cout << "Key: ";
	std::cin >> key;
	updateDepartment(key);
}


void removeFromIndex(int key) {
	FILE* fp;
	fopen_s(&fp, DEPARTMENT_INDEX_FILE, "rb+");
	if (!fp)
		return;
	int size = getNumberOfDepartmentIndex(fp);
	size--;
	DepartmentIndex* departments = new DepartmentIndex[size];

	for (int i = 0; i < size; i++) {
		DepartmentIndex current;
		fread(&current, sizeof(DepartmentIndex), 1, fp);
		if (current.key == key) {
			fread(&current, sizeof(DepartmentIndex), 1, fp);
		}
		departments[i] = current;
	}
	fclose(fp);

	//rewrite the file
	fopen_s(&fp, DEPARTMENT_INDEX_FILE, "wb");
	if (fp) {
		fwrite(departments, sizeof(DepartmentIndex), size, fp);
		fclose(fp);
	}
	delete[] departments;
}

void rewriteDepartment(Department d) {
	DepartmentIndex index = getDepartmentIndex(d.index);
	updateDepartmentFile(index.address, d);
}
void deleteDepartment(int key) {
	DepartmentIndex index = getDepartmentIndex(key);
	if (index.address != -1) {
		addGarbage(index.address, GARBAGE_FILE);
		Department toDelete = findByAddress(index.address);
		toDelete.isDeleted = true;

		//delete teachers [TODO]!!!

		updateDepartmentFile(index.address, toDelete);
		removeFromIndex(key);
	}
	else {
		std::cout << "Can't find this key!" << std::endl;
	}
}

void deleteDepartmentByKey() {
	int key;
	std::cout << "Key: ";
	std::cin >> key;
	deleteDepartment(key);
}

void updateDepartmentFile(int address, Department department) {
	FILE* fp;
	fopen_s(&fp, DEPARTMENT_FL_FILE, "rb+");
	if (fp) {
		fseek(fp, sizeof(Department) * address, SEEK_SET);
		fwrite(&department, sizeof(department), 1, fp); //rewrite previous
		fclose(fp);
	}
	else {
		std::cout << "Can't open file to update!" << std::endl;
	}
}


void printAllDepartments() {
	FILE* fp;
	fopen_s(&fp, DEPARTMENT_INDEX_FILE, "rb");
	if (fp) {
		DepartmentIndex index;
		while (true) {
			fread(&index, sizeof(index), 1, fp);
			if (feof(fp)) {
				break;
			}
			else {
				Department d = findByAddress(index.address);
				if (!d.isDeleted) {
					d.print();
				}
			}
		}
		fclose(fp);
	}
}

void insertDepartment() {
	Department d = createDepartment();
	addToFiles(d);
}

int main(void) {
	return menu();
}




int menu() {
	char command[10];
	while (true) {
		printf("Command: ");
		std::cin >> command;

		if (strcmp(command, "insert_m") == 0 || strcmp(command, "im") == 0) {
			insertDepartment();
		}
		else if (strcmp(command, "insert_s") == 0 || strcmp(command, "is") == 0) {
			insertTeacher();
		}
		else if (strcmp(command, "del_m") == 0 || strcmp(command, "dm") == 0) {
			deleteDepartmentByKey();
		}
		else if (strcmp(command, "del_s") == 0 || strcmp(command, "ds") == 0) {
			removeTeacherByKey();
		}
		else if (strcmp(command, "print_m") == 0 || strcmp(command, "pm") == 0) {
			printDepartment();
		}
		else if (strcmp(command, "print_all_m") == 0 || strcmp(command, "pma") == 0) {
			printAllDepartments();
		}
		else if (strcmp(command, "print_s") == 0 || strcmp(command, "ps") == 0) {
			printTeacher();
		}
		else if (strcmp(command, "print_all_s") == 0 || strcmp(command, "psa") == 0) {
			printAllTeachers();
		}
		else if (strcmp(command, "print_direct_s") == 0 || strcmp(command, "psd") == 0) {
			directPrintTeachers();
		}
		else if (strcmp(command, "update_m") == 0 || strcmp(command, "um") == 0) {
			updateDepartmentByKey();
		}
		else if (strcmp(command, "update_s") == 0 || strcmp(command, "us") == 0) {
			updateTeacherByKey()
		}
		else if (strcmp(command, "help") == 0) {
			std::cout << "HELP:\n\ninsert_m - add new department";
		}
		else if (strcmp(command, "exit") == 0 || strcmp(command, "close") == 0) {
			break;
		}
		else {
			puts("Unknown command\n");
		}
		std::cout << std::endl;
	}
	return 0;
}