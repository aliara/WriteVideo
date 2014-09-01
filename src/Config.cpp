/* 
 * File:   Config.cpp
 * Configuration class for the VidRecord app.
 * Author: Vern Raben
 * Copyright Raben Systems, Inc. 2012, All rights reserved.
 * @license: BSD 2 Clause License. See license included with this distribution in the file "license.txt"
 * Created on October 30, 2012, 5:56 PM
 */

#include "Config.h"
#include <map>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

/**
 * No arg constructor
 */
Config::Config() {
}

/**
 * Destructor, release resources
 */
Config::~Config() {
	vals.clear();
}

/**
  * Get configuration value for a parameter
  * @param name Name of the parameter
  * @param default Default value for the parameter if not set
  * @return the parameter's value as a string
  */
string Config::getVal(string name, string defaultVal) {
	string retVal = defaultVal;

	if (vals.count(name) > 0) {
		retVal = vals[name];
	}

	return retVal;
}

/**
  * Get configuration value for a parameter
  * @param name Name of the parameter
  * @param default Default value for the parameter if not set
  * @return the parameter's value as a boolean
  */
bool Config::getVal(string name, bool defaultVal) {
	bool retVal = defaultVal;

	if (vals.count(name) > 0) {
		string str = vals[name];
		transform(str.begin(), str.end(),str.begin(), ::tolower);
		retVal = str == "true" ? true : false;
	}

	return retVal;
}

/**
  * Get configuration value for a parameter
  * @param name Name of the parameter
  * @param default Default value for the parameter if not set
  * @return the parameter's value as an integer
  */
int Config::getVal(string name, int defaultVal) {
	int retVal = defaultVal;

	if (vals.count(name) > 0) {
		istringstream iss(vals[name]);
		iss >> retVal;
	}

	return retVal;
}

/**
  * Get configuration value for a parameter
  * @param name Name of the parameter
  * @param default Default value for the parameter if not set
  * @return the parameter's value as double
  */
double Config::getVal(string name, double defaultVal) {
	double retVal = defaultVal;

	if (vals.count(name) > 0) {
		istringstream iss(vals[name]);
		iss >> retVal;
	}
	return retVal;
}

/**
 * Add parameter to configuration
 * @param name Name of the parameter
 * @@param val Value of the parameter
 */

void Config::addVal(string name, string val) {
	vals[name] = val;
}

/**
 * Load parameters from file
 * @param filename
 */
void Config::loadFromFile(string filename) {

    ifstream inFil;
    inFil.open(filename.c_str(), ifstream::in);
    string line;

    if (inFil.is_open()) {

        while(!inFil.eof()) {
            string key,xx,val;
            getline(inFil, line);

            if (line.length() > 2) {
                istringstream iss (line, istringstream::in);
                iss >> key >> xx >> val;
                if (key == "#") continue;
                vals[key] = val;
            }

        }

        inFil.close();

    }

}

/**
 * Print out configuration parameter names and values
 */
void Config::printVals() {

	cout << "Config vals:"<<endl;

	for ( map<string,string>::iterator ii=vals.begin(); ii!=vals.end(); ++ii) {
		cout << (*ii).first << ": " << (*ii).second << endl;
	}
}
