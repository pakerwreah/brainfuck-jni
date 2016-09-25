//
// Created by Paker on 16/05/16.
//

#include "Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <dirent.h>

namespace utils {

    using namespace std;

    bool contains(string str, string needle) {
        return str.find(needle)!=string::npos;
    }

    bool endsWith(string str, string needle) {
        int p = str.length() - needle.length();
        return p >=0 && str.substr(p) == needle;
    }

    bool startsWith(string str, string needle){
        return str.find(needle) == 0;
    }

    string readfile(string path) {
        ifstream in(path);
        stringstream buffer;
        buffer << in.rdbuf();
        return buffer.str();
    }

    vector<string> split(string str, string delimiter) {
        vector<string> strings;
        size_t pos = 0;
        while ((pos = str.find(delimiter)) != string::npos) {
            string token = str.substr(0, pos);
            strings.push_back(token);
            str.erase(0, pos + delimiter.length());
        }

        if(str.length())
            strings.push_back(str);

        return strings;
    }

    string join(vector<string>& elements, string delimiter)
    {
        std::stringstream ss;
        size_t elems = elements.size(),
                last = elems - 1;

        for( size_t i = 0; i < elems; ++i )
        {
            ss << elements[i];

            if( i != last )
                ss << delimiter;
        }

        return ss.str();
    }

    string str_replace(string &s, const string &toReplace, const string &replaceWith) {
        auto p = s.find(toReplace);

        if(p!=string::npos)
            return s.replace(p, toReplace.length(), replaceWith);

        return s;
    }

    vector<string> getdir(string dir)
    {
        vector<string> files;
        DIR *dp;
        struct dirent *dirp;
        if((dp  = opendir(dir.c_str())) == NULL)
            cout << "Error(" << errno << ") opening " << dir << endl;

        while ((dirp = readdir(dp)) != NULL)
            if(!startsWith(dirp->d_name,"."))
                files.push_back(string(dirp->d_name));

        closedir(dp);
        return files;
    }

    int stoi(std::string str)
    {
        std::stringstream ss(str);
        int N;
        ss<<str;
        ss>>N;
        return N;
    }
}
