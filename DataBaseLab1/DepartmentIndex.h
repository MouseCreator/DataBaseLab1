#pragma once

struct DepartmentIndex {
	int key;
	int address;
	DepartmentIndex(int key = -1, int address = -1) {
		this->key = key;
		this->address = address;
	}
};