#include <windows.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define CP_SJIS 932

std::string Utf8ToShiftjis(const std::string str) {
	const int size_unicode = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	wchar_t* p_unicode = new wchar_t[size_unicode];
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, p_unicode, size_unicode);
	
	const int size_shiftjis = WideCharToMultiByte(CP_SJIS, 0, p_unicode, -1, nullptr, 0, nullptr, nullptr);
	char* p_shiftjis = new char[size_shiftjis];
	WideCharToMultiByte(CP_SJIS, 0, p_unicode, -1, p_shiftjis, size_shiftjis, nullptr, nullptr);
	std::string res = std::string(p_shiftjis);

	delete[] p_shiftjis;
	p_shiftjis = nullptr;
	delete[] p_unicode;
	p_unicode = nullptr;
	return res;
}

int main(int argc, char** argv) {
    std::ifstream ifs = std::ifstream("./resource.rcmap");
    if (ifs.fail())
        return 1;
    std::ofstream rc = std::ofstream("./resource.rc", std::ios::out);
    if (rc.fail())
        return 1;
    std::ofstream hpp = std::ofstream("../src/_resource.hpp", std::ios::out);
    if (hpp.fail())
        return 1;
    rc << "#include \"../src/_resource.hpp\"\n\n";
    hpp << "#pragma once\n#ifndef _RESOURCE_H_\n#define _RESOURCE_H_\n\n";
    unsigned int cnt = 1;
    std::string ftype = "";
    std::string buf = "";
    while (std::getline(ifs, buf)) {
        if (buf[0] == '#') {
            if (ftype != "")
                hpp << "#define RES_IDX_" << ftype << "_END " << (cnt - 1) << "\n\n";
            ftype = buf.substr(1, buf.size() - 1);
            hpp << "#define RES_IDX_" << ftype << "_START " << cnt << "\n";
            continue;
        } else if (buf[0] == '$') {
            std::istringstream iss = std::istringstream(buf.substr(1, buf.size() - 1));
            std::string tmp;
            std::vector<std::string> strs;
            while (std::getline(iss, tmp, ':')) {
                strs.push_back(tmp);
            }
            hpp << "#define " << strs.at(0) << " " << cnt << "\n";
            rc << strs.at(0) << " " << ftype;
            for (int i = 1; i < strs.size(); ++i) {
                rc << " " << Utf8ToShiftjis(strs.at(i));
            }
            rc << "\n";
            ++cnt;
            continue;
        }
        std::istringstream iss = std::istringstream(buf);
        std::string id, filename;
        iss >> id >> filename;
        rc << id << " " << ftype << " "
           << "\"" << Utf8ToShiftjis(filename) << "\"\n";
        hpp << "#define " << id << " " << cnt << "\n";
        ++cnt;
    }
    if (ftype != "")
        hpp << "#define RES_IDX_" << ftype << "_END " << (cnt - 1) << "\n\n";
    hpp << "#endif\n";
    hpp.close();
    rc.close();
    ifs.close();
    return 0;
}