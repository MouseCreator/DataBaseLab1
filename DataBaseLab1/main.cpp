
#include "fuctions.h"



int main(void) {
	return menu();
}

int menu() {
	char command[20];
	while (true) {
		printf("Command: ");
		std::cin >> command;

		//INSERT

		if (strcmp(command, "insert_m") == 0 || strcmp(command, "im") == 0) {
			insertDepartment();
		}
		else if (strcmp(command, "insert_s") == 0 || strcmp(command, "is") == 0) {
			insertTeacher();
		}

		//DELETE

		else if (strcmp(command, "del_m") == 0 || strcmp(command, "dm") == 0) {
			deleteDepartmentByKey();
		}
		else if (strcmp(command, "del_s") == 0 || strcmp(command, "ds") == 0) {
			removeTeacherByKey();
		}

		//PRINT

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
		else if (strcmp(command, "print_everything") == 0 || strcmp(command, "pe") == 0) {
			printEverything();
		}

		//UPDATE

		else if (strcmp(command, "update_m") == 0 || strcmp(command, "um") == 0) {
			updateDepartmentByKey();
		}
		else if (strcmp(command, "update_s") == 0 || strcmp(command, "us") == 0) {
			updateTeacherByKey();
		}

		//OTHER

		else if (strcmp(command, "help") == 0) {
			std::cout << "\tHELP:\n" <<
				"\tinsert_m - add new department\n" <<
				"\tinsert_s - add new teacher\n" <<
				"\tupdate_m - update department\n" <<
				"\tupdate_s - update teacher\n" <<
				"\tdel_m - delete department (and all teachers in it)\n" <<
				"\tdel_s - delete teacher from department\n" <<
				"\tprint_m - print department info\n" <<
				"\tprint_s - print teacher info\n" <<
				"\tprint_all_m - print all departments\n" <<
				"\tprint_all_s - print all teachers of the department\n" <<
				"\tprint_everything - print all departments and teachers\n" <<
				"\tprint_direct_s - print all teachers of all departments\n" <<
				"\texit / close - leave program\n";
		}
		else if (strcmp(command, "exit") == 0 || strcmp(command, "close") == 0) {
			break;
		}
		else {
			std::cout << "Unknown command" << std::endl;
		}
		std::cout << std::endl;
	}
	return 0;
}

