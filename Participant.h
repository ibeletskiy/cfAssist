#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <set>

class Task {
private:
	bool verdict, solving_type;
	int contest_id, rating;
	std::set<std::string> tags;
	std::string handle, name;
public:
	Task(const std::string& task);
	bool GetVerdict();
	std::set<std::string> GetTags();
	int GetRating();
	std::string GetHandle();
	std::string GetTaskName();
	bool GetSolvingType();
};

class Participant {
private:
	int all_count, solved_count;
	std::vector<Task> tasks;
	std::string handle;
	bool CF_Avaliable;
	bool status;
	int rating;
public:
	Participant(const std::string& file_name);
	int GetAllTask();
	int GetOKTask();
	std::vector<Task> ListAllTask();
	bool GetCFStatus();
	void RemakeInputFile(const std::string& input_file);
	std::pair<int, int> TagCount(const std::string& tag);
	std::string GetHandle();
	int GetRating(const std::string& file_name);
};

double GetAverageRating(Participant& person, std::string& type_task) {
    std::set<std::string> was;
    int OK = 0, ALL = 0, SM = 0;
    for (Task x : person.ListAllTask()) {
        if (was.find(x.GetTaskName()) != was.end()) {
            continue;
        }
        auto ids = x.GetTags();
        if (ids.find(type_task) != ids.end()) {
            ALL++;
            if (x.GetVerdict()) {
                OK++;
                SM += x.GetRating();
            }
            was.insert(x.GetTaskName());
        }
    }
    if (!OK) return { INF };
    return { SM / (double)OK };
}


double GetStability(Participant& person, std::string& type_task) {
    std::set<std::string> was;
    int OK = 0, ALL = 0, SM = 0;
    for (Task x : person.ListAllTask()) {
        if (was.find(x.GetTaskName()) != was.end()) {
            continue;
        }
        auto ids = x.GetTags();
        if (ids.find(type_task) != ids.end()) {
            ALL++;
            if (x.GetVerdict()) {
                OK++;
                SM += x.GetRating();
            }
            was.insert(x.GetTaskName());
        }
    }
    if (!ALL) return { INF };
    return { OK / (double)ALL };
}

int GetOK(Participant& person, std::string& type_task) {
    std::set<std::string> was;
    int OK = 0, ALL = 0, SM = 0;
    for (Task x : person.ListAllTask()) {
        if (was.find(x.GetTaskName()) != was.end()) {
            continue;
        }
        auto ids = x.GetTags();
        if (ids.find(type_task) != ids.end()) {
            ALL++;
            if (x.GetVerdict()) {
                OK++;
                SM += x.GetRating();
            }
            was.insert(x.GetTaskName());
        }
    }
    return OK;
}


int GetALL(Participant& person, std::string& type_task) {
    std::set<std::string> was;
    int OK = 0, ALL = 0, SM = 0;
    for (Task x : person.ListAllTask()) {
        if (was.find(x.GetTaskName()) != was.end()) {
            continue;
        }
        auto ids = x.GetTags();
        if (ids.find(type_task) != ids.end()) {
            ALL++;
            if (x.GetVerdict()) {
                OK++;
                SM += x.GetRating();
            }
            was.insert(x.GetTaskName());
        }
    }
    return ALL;
}
