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
int argumentProcessor(const char **homedir, char **argv, string *funnel_type);
bool nameCompare(string input, string search_name);
int vectorContains(vector<Object> input, string name);
string promptUserChoice(vector<string> paths, string name);
bool funnelCheck(struct dirent **entry, string funnel_type);
void printObjectVector(vector<Object> input);


/****************************************
			Finder.cpp
*****************************************/
void findObject(const char *name, vector<Object> &object, string funnel_type);

#endif