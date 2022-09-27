#include "fuctions.h"

const char* TEACHER_FL_FILE = "teacher.fl";
const char* TEACHER_GARBAGE_FILE = "teacher_garbage.fl";

int insertToFile(Teacher teacher, int firstPosition) {
	FILE* fp;
	fopen_s(&fp, TEACHER_FL_FILE, "rb+");
	if (!fp) {
		std::cout << "Can't open " << TEACHER_FL_FILE << std::endl;
		return -1;
	}
	teacher.next = firstPosition;

	int result = writeTeacher(fp, teacher);
	
	fclose(fp);
	return result;
}

int writeTeacher(FILE* fp, Teacher teacher) {
	int addTo = getUnusedIndex(TEACHER_GARBAGE_FILE);

	int newPosition;
	if (addTo == -1) {
		fseek(fp, 0, SEEK_END);
	}
	else {
		fseek(fp, sizeof(Teacher) * addTo, SEEK_SET);
	}
	newPosition = ftell(fp) / sizeof(Teacher);
	fwrite(&teacher, sizeof(teacher), 1, fp);
	return newPosition;
}
void writeTeacher(FILE* fp, Teacher teacher, int position) {

	fseek(fp, sizeof(Teacher) * position, SEEK_SET);
	fwrite(&teacher, sizeof(teacher), 1, fp);
}

Teacher readTeacher(FILE* fp, int position) {
	Teacher teacher;
	fseek(fp, position * sizeof(Teacher), SEEK_SET);
	fread(&teacher, sizeof(teacher), 1, fp);
	return teacher;
}

bool hasTeacher(int key, int firstTeacherAddress) {
	int current = firstTeacherAddress;
	FILE* fp;
	
	bool result = false;
	fopen_s(&fp, TEACHER_FL_FILE, "rb");
	if (!fp) {
		std::cout << "Can't open " << TEACHER_FL_FILE << std::endl;
		return false;
	}
	while (current != -1) {
		Teacher teacher = readTeacher(fp, current);
		if (teacher.tabnumber == key) {
			result = true;
			break;
		}
		current = teacher.next;
	}
	fclose(fp);
	return result;
}
Teacher getTeacher(int key, int firstTeacherAddress) {
	int current = firstTeacherAddress;
	FILE* fp;
	Teacher result;
	fopen_s(&fp, TEACHER_FL_FILE, "rb");
	if (!fp) {
		std::cout << "Can't open " << TEACHER_FL_FILE << std::endl;
		return result;
	}
	while (current != -1) {
		Teacher teacher = readTeacher(fp, current);
		if (teacher.tabnumber == key) {
			result = teacher;
			break;
		}
		current = teacher.next;
	}
	fclose(fp);
	return result;
}
Teacher createTeacher(int firstTeacherAddress) {
	Teacher toCreate;
	do {
		std::cout << "Tabnumber: ";
		std::cin >> toCreate.tabnumber;
	} while (hasTeacher(toCreate.tabnumber, firstTeacherAddress) || toCreate.tabnumber == -1);

	changeTeacherFields(toCreate);
	return toCreate;
}

Department findDepartment() {
	int departmentKey;
	std::cout << "Department: ";
	std::cin >> departmentKey;
	return getDepartment(departmentKey);
}

void changeTeacherFields(Teacher& teacher, bool updateMode) {
	Teacher prev = teacher;

	std::cout << "Name: ";

	std::cin.ignore();
	std::cin.getline(teacher.name, TEACHER_NAME_LENGTH);
	if (updateMode && strcmp(teacher.name, "") == 0) {
		strcpy_s(teacher.name, prev.name);
	}

	std::cout << "E-mail: ";
	std::cin >> teacher.email;
	if (updateMode && strcmp(teacher.email, "") == 0) {
		strcpy_s(teacher.email, prev.email);
	}

	std::cout << "Degree: ";
	std::cin >> teacher.degree;
	if (updateMode && strcmp(teacher.degree, "") == 0) {
		strcpy_s(teacher.degree, prev.degree);
	}

	std::cout << "Scientific title: ";
	std::cin >> teacher.scintific_title;
	if (updateMode && strcmp(teacher.scintific_title, "") == 0) {
		strcpy_s(teacher.scintific_title, prev.scintific_title);
	}
}

void insertTeacher() {

	Department d = findDepartment();
	if (d.index == -1) {
		std::cout << "Can't find this department!" << std::endl;
		return;
	}
	Teacher t = createTeacher(d.firstTeacherNumber);
	int inserted = insertToFile(t, d.firstTeacherNumber);
	if (inserted != -1) //success
		d.firstTeacherNumber = inserted;
	rewriteDepartment(d);
}

void printTeacher() {
	int departmentKey;
	std::cout << "Department: ";
	std::cin >> departmentKey;

	int teacherKey;
	std::cout << "Teacher: ";
	std::cin >> teacherKey;

	Department d = getDepartment(departmentKey);
	Teacher t = getTeacher(teacherKey, d.firstTeacherNumber);
	t.print();
}

void printAllTeachers() {

	
	Department d = findDepartment();

	printTeachers(d);
}

void printTeachers(Department d) {
	FILE* fp;
	fopen_s(&fp, TEACHER_FL_FILE, "rb");
	if (!fp)
		return;
	int current = d.firstTeacherNumber;
	if (current == -1) {
		std::cout << "No teachers in the department." << std::endl;
	}
	while (current != -1) {
		Teacher t = readTeacher(fp, current);
		current = t.next;
		t.print();
	}

	fclose(fp);
}

void directPrintTeachers() {
	FILE* fp;
	fopen_s(&fp, TEACHER_FL_FILE, "rb");
	if (!fp)
		return;
	Teacher toPrint;
	while (true) {
		fread(&toPrint, sizeof(Teacher), 1, fp);
		if (feof(fp))
			break;
		if (!toPrint.isDeleted)
			toPrint.print();
	}
	fclose(fp);
}

int removeTeacher(int key, int first) {
	FILE* fp;

	fopen_s(&fp, TEACHER_FL_FILE, "rb+");
	if (!fp) {
		std::cout << "Can't open file " << TEACHER_FL_FILE << std::endl;
		return first;
	}
	int result = first;
	int prevAddress = -1;
	Teacher current;
	int nowReading = first;
	while (nowReading != -1) {
		current = readTeacher(fp, nowReading);
		if (current.tabnumber == key) {
			//markers
			addGarbage(nowReading, TEACHER_GARBAGE_FILE);
			current.isDeleted = true;
			
			//change pointers
			if (prevAddress != -1) {
				Teacher prevTeacher = readTeacher(fp, prevAddress);
				prevTeacher.next = current.next;
				writeTeacher(fp, prevTeacher, prevAddress);
			}
			else {
				result = current.next;
			}

			//rewrite teacher
			writeTeacher(fp, current, nowReading);
		}
		prevAddress = nowReading;
		nowReading = current.next;
	}
	fclose(fp);
	return result;
}

void removeTeacherByKey() {
	Department d = findDepartment();
	if (d.index == -1) {
		std::cout << "No such department in the bese!" << std::endl;
		return;
	}
	int teacherKey;
	std::cout << "Teacher: ";
	std::cin >> teacherKey;

	d.firstTeacherNumber = removeTeacher(teacherKey, d.firstTeacherNumber);
	rewriteDepartment(d);
}

void delAllTeachers(Department d) {
	FILE* fp;

	fopen_s(&fp, TEACHER_FL_FILE, "r+");

	if (!fp) {
		std::cout << "Unable to delete all department's teachers. Might cause memory leak!" << std::endl;
		return;
	}
	int current = d.firstTeacherNumber;
	while (current != -1) {
		Teacher teacher;
		fread(&teacher, sizeof(Teacher), 1, fp);
		addGarbage(current, TEACHER_GARBAGE_FILE);
		teacher.isDeleted = true;
		writeTeacher(fp, teacher, current);
		current = teacher.next;
	}
	fclose(fp);

}


void updateTeacherByKey() {
	Department d = findDepartment();
	int key;
	std::cout << "Teacher: ";
	std::cin >> key;

	updateTeacher(key, d.firstTeacherNumber);
}

void updateTeacher(int key, int first) {
	Teacher t = getTeacher(key, first);
	changeTeacherFields(t, true);
	rewriteTeacher(t, first);
}

void rewriteTeacher(Teacher t, int first) {
	FILE* fp;
	fopen_s(&fp, TEACHER_FL_FILE, "r+");
	if (!fp) {
		std::cout << "Can't update teacher!" << std::endl;
		return;
	}
	int current = first;
	while (current != -1) {
		Teacher last;
		int toWrite = ftell(fp) / sizeof(Teacher);
		fread(&last, sizeof(Teacher), 1, fp);
		if (last.tabnumber == t.tabnumber) {
			writeTeacher(fp, t, toWrite);
			break;
		}
	}
	fclose(fp);
}

