#ifndef FINDER_H
#define FINDER_H

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include "object.cpp"
#include "helper.cpp"

using namespace std;

/****************************************
			Helper.cpp
*****************************************/
void copyInput(vector<Object> &input, char *argv[], int i);
int getStartingDir(const char **homedir, char **argv);
bool nameCompare(string input, string search_name);
int vectorContains(vector<Object> input, string name);
string promptUserChoice(vector<string> paths, string name);


/****************************************
			Finder.cpp
*****************************************/
void findObject(const char *name, int level, vector<Object> &object);

#endif