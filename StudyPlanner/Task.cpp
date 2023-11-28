#include "Task.h"
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <cstring>


std::vector<Task> DisplayTasks;

void saveTasksToFile(const std::vector<Task>& tasks, const std::string& fileName) {
	std::ofstream ostream(fileName);
	ostream << tasks.size();
	for (const Task& task : tasks) {
		std::string description = task.description;
		std::replace(description.begin(), description.end(), ' ', '_'); // to replace ever space in tasks with underscore
		ostream << '\n' << task.date << ' ' << description << ' ' << task.done;
	}
}



std::vector<Task> loadTasksFromFile(const std::string fileName, const std::string passedDate) {
	DisplayTasks.clear();
	if (!std::filesystem::exists(fileName)) {
		return std::vector<Task>();
	}
	std::ifstream istream(fileName);


	int num;
	istream >> num;

	for (int i = 0; i < num; i++) {

		std::string desc, date;
		bool done;
		istream >> date >> desc >> done;
		if (strcmp(date.c_str(), passedDate.c_str()) == 0) {
			std::replace(desc.begin(), desc.end(), '_', ' ');
			DisplayTasks.push_back(Task{ date, desc, done });
		}

	}

	return DisplayTasks;
}

std::vector<int> pendingtaskCount(const std::string fileName)
{
	std::vector<int> count;
	if (!std::filesystem::exists(fileName)) {
		return std::vector<int>();
	}
	int c, p;
	std::ifstream istream(fileName);
	istream >> c;
	count.push_back(c);
	p = 0;
	for (int i = 0; i < c; i++) {

		std::string desc, date;
		bool donee;
		istream >> date >> desc >> donee;
		if (!donee) p++;
	}
	count.push_back(p);
	return count;
}

//OnAddButtonClicked()
