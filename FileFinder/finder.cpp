#include "finder.h"

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
	for (int x = 0; x < input.size(); x++) {
		if (input.at(x).getPaths().size() > 1) {
			pathnames.push_back(promptUserChoice(input.at(x).getPaths(), input.at(x).getName()));
		}
		else if (input.at(x).getPaths().size() == 1) {
			pathnames.push_back(input.at(x).getPaths().at(0));
		}
		else {
			cout << "Could not find a file or directory with the given name." << endl;
			cout << "Please try again" << endl;
			return 0;
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

			if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
				continue;

			i = vectorContains(input, string(entry->d_name));

			//Checks to see if the entry name is in the input vector
			if (i >= 0) {

					//Gets rid of extra '/' when starting from /
					if (path[0] == '/' && path[1] == '/') {
						memcpy(path, &path[1], sizeof(path) - 1);
					}

					input.at(i).addPath(string(path));
			}

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