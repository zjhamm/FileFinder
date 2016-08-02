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

using namespace std;

void copyInput(vector<Object> &input, char *argv[], int i);
int getStartingDir(const char **homedir, char **argv);
void findObject(const char *name, int level, vector<Object> &object);
string promptUserChoice(vector<string> paths, string name);

int main(int argc, char *argv[])
{
	int i;
	const char *startdir;
	vector<string> pathnames;
	vector<Object> input;

	//Check to make sure that the user specified a file or directory
	if (argc == 1) {
		printf("Input Invalid: Need to specify a directory or a file\n");
		return 0;
	}

	//Decides if we are starting from root or home
	i = getStartingDir(&startdir, argv);

	//Copy the input from the user into our input vector
	copyInput(input, argv, i);

	//Finds the object in the file system
	//Starting at the given dirname
	findObject(startdir, 0, input);

	//Request user to choose path if multiple
	cout << input.size() << endl;
	for (int x = 0; x < input.size(); x++) {
		if (input.at(x).getPaths().size() > 1) {
			pathnames.push_back(promptUserChoice(input.at(x).getPaths(), input.at(x).getName()));
		}
		else {
			pathnames.push_back(input.at(x).getPaths().at(0));
		}
	}

	for (int x = 0; x < pathnames.size(); x++){
		cout << "Opening " + pathnames.at(x) + " in sublime" << endl;
	}

	cout << endl;

	string sublime_command = "subl ";

	for (int x = 0; x < pathnames.size(); x++) {
		sublime_command.append(pathnames.at(x) + " ");
	}

	sublime_command.append(" &");

	cout << "Command: " << sublime_command << endl;

	system(sublime_command.c_str());
}

/**************************************************

					FUNCTIONS

**************************************************/

void copyInput(vector<Object> &input, char *argv[], int i) {

	//Loop through the input creating objects
	//Saving them to our vector	

	while(argv[i] != "" && argv[i] != 0) {
		Object temp = Object(string(argv[i]));
		input.push_back(temp);
		i++;
	}
}

int getStartingDir(const char **homedir, char ** argv) {

	int j = 1;

	while(argv[j] != "" && argv[j] != 0) {
		if (strcmp(argv[j], "--root") == 0) {
			cout << "Starting from root" << endl;
			*homedir = "/";
			j++;
			return j;
		}
		else if (argv[j][0] != '-') {
			break;
		}

		j++;
	}

	//Get the home directory of the current user
	if((*homedir = getenv("HOME")) == NULL) {
		*homedir = getpwuid(getuid())->pw_dir;
	}

	return j;
}

int vectorContains(vector<Object> input, string name) {
	for (int i = 0; i < input.size(); i++) {
		if (input.at(i).getName() == name) {
			return i;
		}
	}

	return -1;
}

string promptUserChoice(vector<string> paths, string name) {

	int input;

	cout << "Found multiple files or directories with the name: " << name << endl;
	cout << "Please choose from the following list which you would like to open: " << endl;

	for (int i = 0; i < paths.size(); i++) {
		cout << i + 1 << ": " << paths.at(i) << endl;
	}

	cout << endl << "Input: ";
	cin >> input;

	return paths.at(input - 1);
}

void findObject(const char *name, int level, vector<Object> &input) {

	DIR *dir;
	struct dirent *entry;

	//Check to make sure the default directory can be opened
	//In my case it is HOME
	if(!(dir = opendir(name)))
		return;

	//Checks to make sure that the dir is readable
	if(!(entry = readdir(dir)))
		return;

	do {
		//If the entry type is Directory
		if(entry->d_type == DT_DIR) {
			
			char path[1024];
			int i;
			int len = snprintf(path, sizeof(path) - 1, "%s/%s", name, entry->d_name);
			path[len] = 0;

			i = vectorContains(input, string(entry->d_name));

			//Checks to see if the entry name is in the input vector
			if (i >= 0) {

					//Gets rid of extra '/' when starting from /
					if (path[0] == '/' && path[1] == '/') {
						memcpy(path, &path[1], sizeof(path) - 1);
					}

					input.at(i).addPath(string(path));
			}

			if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
				continue;

			//printf("%*s[%s]\n", level*2, "", entry->d_name);

			findObject(path, level + 1, input);
		}
		else {

			char path[1024];
			int i;
			int len = snprintf(path, sizeof(path) - 1, "%s/%s", name, entry->d_name);
			path[len] = 0;

			i = vectorContains(input, string(entry->d_name));

			//Checks to see if the entry name is in the input vector
			if (i >= 0) {

					//Gets rid of extra '/' when starting from /
					if (path[0] == '/' && path[1] == '/') {
						memcpy(path, &path[1], sizeof(path) - 1);
					}

					input.at(i).addPath(string(path));
					continue;
			}
		}
	} while (entry = readdir(dir));

	closedir(dir);
}