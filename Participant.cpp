#include "Participant.h"


Task::Task(const std::string &task) {
    int index;
    index = task.find("contestId");
    index += 11;
    while (task[index] != ',') {
        contest_id = contest_id * 10 + task[index] - '0';
        ++index;
    }
    index = task.find("rating");
    index += 8;
    while (task[index] != ',') {
        rating = rating * 10 + task[index] - '0';
        ++index;
    }
    index = task.find("tags");
    index += 8;
    while (task[index - 1] != '}') {
        std::string cur_tag;
        while (task[index] != '\"') {
            cur_tag += task[index];
            ++index;
        }
        tags.insert(cur_tag);
        index += 3;
    }
    index = task.find("verdict");
    index += 7 + 3;
    if (task[index] == 'O') verdict = true;
    else verdict = false;
    index = task.find("handle");
    index += 9;
    while (task[index] != '\"') {
        handle += task[index];
        ++index;
    }
    int a = 0;
}

bool Task::GetVerdict() {
    return verdict;
}

std::set<std::string> Task::GetTags() {
    return tags;
}

int Task::GetRating() {
    return rating;
}

std::string Task::GetHandle(){
    return handle;
}

std::string Participant::GetHandle(){
    return handle;
}

void Participant::RemakeInputFile(const std::string& input_file){
    std::fstream in(input_file);
    std::string text;
    std::getline(in, text);
    if (text[0] == '<'){
        CF_Avaliable = false;
        return;
    }
    std::ofstream out("new_input_file.txt");
    int index = text.find("FAILED");
    if (index >= 0 && index < text.size()){
        status = false;
        return;
    }
    index = text.find('[');
    text.erase(0, index + 1);
    while (text.size() >= 1) {
        index = text.find("memoryConsumedBytes");
        while (text[index] != ',') ++index;
        text.substr(0, index + 1);
        out << text << '\n';
        text.erase(0, index + 1);
    }
}

Participant::Participant(const std::string &file_name) {
    RemakeInputFile(file_name);
    if (!status || !CF_Avaliable) {
        return;
    }
    std::string line;
    std::fstream in("new_input_file.txt");
    while (std::getline(in, line)){
        Task cur_task(line);
        tasks.push_back(cur_task);
        all_count++;
        if (cur_task.GetVerdict()) solved_count++;
        handle = cur_task.GetHandle();
    }
}

int Participant::GetAllTask() {
    return all_count;
}

int Participant::GetOKTask() {
    return solved_count;
}

std::vector<Task> Participant::ListAllTask() {
    return tasks;
}

bool Participant::GetCFStatus() {
    return CF_Avaliable;
}

std::pair<int, int> Participant::TagCount(const std::string &tag) { // first - OK, second - all
    std::pair<int, int> ret {0, 0};
    for (auto task : tasks){
        if (task.GetVerdict()) {
            ret.first++;
            ret.second++;
        } else {
            ret.second++;
        }
    }
    return ret;
}
