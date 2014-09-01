/* 
 * File:   Config.h
 * Header for Config.cpp.
 * Author: Vern Raben
 * Copyright Raben Systems, Inc. 2012, All rights reserved.
 * @license: BSD 2 Clause License. See license included with this distribution in the file "license.txt"
 * Created on October 30, 2012, 5:56 PM
 */

#ifndef CONFIG_H
#define	CONFIG_H
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

class Config {
public:
    Config();
    virtual ~Config();
    string getVal(string name, string defaultVal);
    bool getVal(string name, bool defaultVal);
    int getVal(string name, int defaultVal);
    double getVal(string name, double defaultVal);
    void addVal(string name, string val);
    void loadFromFile(string file);
    void printVals();    
private:
    map <string, string> vals;
};

#endif	/* CONFIG_H */

