#include <windows.h>

#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#pragma comment(lib, "shell32.lib")

struct NameTime {
    std::string name;
    unsigned long long time;
};

void AddNeeds(
    std::vector<std::string>* needs, std::vector<NameTime>* nts, std::string filename, unsigned long long filetime) {
    for (int i = 0; i < nts->size(); ++i) {
        if (nts->at(i).name == filename && nts->at(i).time >= filetime)
            return;
    }
    std::string filename_new = " ..\\..\\src\\" + filename;
    needs->push_back(filename_new);
}

int main() {
    std::ifstream ifs("./_time");
    if (ifs.fail()) {
        std::cout << "'_time' not found";
        return 1;
    }

    std::vector<NameTime> nts{};
    std::string buf;
    while (std::getline(ifs, buf)) {
        NameTime tmp;
        tmp.name = buf;
        std::getline(ifs, buf);
        tmp.time = stoll(buf);
        nts.push_back(tmp);
    }

    ifs.close();

    std::vector<std::string> needs{};
    std::vector<std::string> alls{};
    std::vector<NameTime> nts_new{};

    WIN32_FIND_DATAA data;
    HANDLE h_find = FindFirstFileA("..\\src\\*", &data);
    if (h_find == INVALID_HANDLE_VALUE) {
        std::cout << "No file found." << std::endl;
        return 1;
    }
    do {
        if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            continue;
        std::string filename(data.cFileName);
        std::istringstream iss(filename);
        std::string former, ext;
        std::getline(iss, former, '.');
        std::getline(iss, ext, '.');
        if (ext != "cpp")
            continue;
        std::string path = "..\\src\\" + filename;
        HANDLE h_file = CreateFileA(
            path.c_str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 0, OPEN_EXISTING, 0, nullptr);
        if (h_file == INVALID_HANDLE_VALUE) {
            std::cout << filename << " not opened." << std::endl;
            continue;
        }
        FILETIME time;
        GetFileTime(h_file, nullptr, nullptr, &time);
        unsigned long long filetime =
            ((unsigned long long)time.dwHighDateTime << 32) | (unsigned long long)time.dwLowDateTime;
        NameTime tmp = {filename, filetime};
        nts_new.push_back(tmp);
        alls.push_back(" .\\" + former + ".obj");
        AddNeeds(&needs, &nts, filename, filetime);
    } while (FindNextFileA(h_find, &data));

    std::ofstream ofs("./_time", std::ios::out);
    if (ofs.fail()) {
        std::cout << "Failed to open _time to output.";
        return 1;
    }
    for (int i = 0; i < nts_new.size(); ++i) {
        ofs << nts_new.at(i).name << "\n";
        ofs << nts_new.at(i).time << "\n";
    }
    ofs.close();

    if (needs.size() <= 0) {
        std::cout << "no file needs compiled." << std::endl;
        return 0;
    }

    std::string opt_file =
        std::accumulate(needs.begin() + 1, needs.end(), needs.at(0), [](std::string& init, std::string& v) {
            return init + v;
        });
    std::string opt_objs =
        std::accumulate(alls.begin() + 1, alls.end(), alls.at(0), [](std::string& init, std::string& v) {
            return init + v;
        });

    if (!ShellExecuteA(nullptr, "open", ".\\objs\\_build1.bat", opt_file.c_str(), "", SW_SHOWNORMAL)) {
        std::cout << "fail" << std::endl;
        return 1;
    }

    int a = getchar();

    if (!ShellExecuteA(nullptr, "open", ".\\objs\\_build2.bat", opt_objs.c_str(), "", SW_SHOWNORMAL)) {
        std::cout << "fail" << std::endl;
        return 1;
    }

    return 0;
}
