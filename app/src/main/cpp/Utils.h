//
// Created by Paker on 16/05/16.
//

#pragma once
#ifndef _Utils_H_
#define _Utils_H_

#include <string>
#include <vector>
#include <sstream>

namespace utils {

    std::string readfile(std::string path);
    std::vector<std::string> split(std::string str, std::string delimiter);
    std::string join(std::vector<std::string>& elements, std::string delimiter);
    std::string str_replace(std::string &s, const std::string &toReplace, const std::string &replaceWith);
    std::vector<std::string> getdir(std::string dir);
    bool contains(std::string str,std::string needle);
    bool endsWith(std::string str,std::string needle);
    bool startsWith(std::string str,std::string needle);
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream ss;
        ss << n;
        return ss.str();
    }
    int stoi(std::string str);
}

#endif
