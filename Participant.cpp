#include "Participant.h"

const int INF = -1e9; // в случае если не определенности

Task::Task(const std::string& task) {
    int index;
    index = task.find("contestId");
    index += 11;
    contest_id = 0;
    while (task[index] != ',') {
        contest_id = contest_id * 10 + task[index] - '0';
        ++index;
    }
    index = task.find("rating");
    index += 8;
    rating = 0;
    while (task[index] != ',') {
        rating = rating * 10 + task[index] - '0';
        ++index;
    }
    index = task.find("tags");
    while (task[index] != '[') ++index;
    if (task[index + 1] != ']') {
        index += 2;
        while (task[index] != ']') {
            std::string cur_tag;
            while (task[index] != '\"') {
                cur_tag += task[index];
                ++index;
            }
            tags.insert(cur_tag);
            ++index;
        }
    }
    if (tags.find(",") != tags.end()) tags.erase(",");
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
    index = task.find("name");
    while (task[index] != '\"') ++index;
    index += 3;
    while (task[index] != '\"') {
        name += task[index];
        ++index;
    }
    index = task.find("participantType");
    while (task[index] != '\"') {
        ++index;
    }
    index += 3;
    if (task[index] == 'P' && task[index + 1] == 'A') {
        solving_type = true;
    } else {
        solving_type = false;
    }
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

std::string Task::GetHandle() {
    return handle;
}

bool Task::GetSolvingType() {
    return solving_type;
}

std::string Participant::GetHandle() {
    return handle;
}

void Participant::RemakeInputFile(const std::string& input_file) {
    std::fstream in(input_file);
    std::string text;
    std::getline(in, text);
    status = CF_Avaliable = true;
    if (text[0] == '<') {
        CF_Avaliable = false;
        return;
    }
    std::ofstream out("new_input_file.txt");
    int index = text.find("FAILED");
    if (index >= 0 && index < text.size()) {
        status = false;
        return;
    }
    if (text.size() < 100) {
        std::ofstream out("new_input_file.txt");
        out << text;
        out.close();
        return;
    }
    index = text.find('[');
    text.erase(0, index + 1);
    while (text.size() >= 1) {
        index = text.find("memoryConsumedBytes");
        while (text[index] != ',' && index < text.size()) ++index;
        auto str = text.substr(0, index + 1);
        out << str << '\n';
        text.erase(0, index + 1);
    }
    in.close();
    out.close();
}

Participant::Participant(const std::string& file_name) {
    RemakeInputFile(file_name);
    if (!status || !CF_Avaliable) {
        return;
    }
    all_count = solved_count = 0;
    std::string line;
    std::fstream in("new_input_file.txt");
    while (std::getline(in, line)) {
        if (line.size() < 100) continue;
        Task cur_task(line);
        tasks.push_back(cur_task);
        all_count++;
        if (cur_task.GetVerdict()) solved_count++;
        handle = cur_task.GetHandle();
    }
    in.close();
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

std::string Task::GetTaskName() {
    return name;
}

std::pair<int, int> Participant::TagCount(const std::string& tag) { // first - OK, second - all
    std::pair<int, int> ret{ 0, 0 };
    for (auto task : tasks) {
        if (task.GetVerdict()) {
            ret.first++;
            ret.second++;
        } else {
            ret.second++;
        }
    }
    return ret;
}

int Participant::GetRating(const std::string& file_name) {
    std::fstream in(file_name);
    std::string text;
    std::getline(in, text);
    int index = text.find("FAILED");
    if (index >= 0 && index < text.size()) {
        return -1;
    }
    index = text.find("rating");
    while (text[index] < '0' || text[index] > '9') ++index;
    rating = 0;
    while (text[index] >= '0' && text[index] <= '9') {
        rating = rating * 10 + text[index] - '0';
        ++index;
    }
    return rating;
}
