#pragma once
#include <fstream>
#include <vector>
#include <set>

const int INF = 1e9;

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
