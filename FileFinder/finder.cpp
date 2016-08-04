#include "finder.h"

int main(int argc, char *argv[])
{
	int i;
	const char *startdir;
	string funnel_type = "";
	vector<string> pathnames;
	vector<Object> input;

	//Check to make sure that the user specified a file or directory
	if (argc == 1) {
		printf("Input Invalid: Need to specify a directory or a file\n");
		return 0;
	}

	//Decides if we are starting from root or home
	i = argumentProcessor(&startdir, argv, &funnel_type);

	//Copy the input from the user into our input vector
	copyInput(input, argv, i);

	//Finds the object in the file system
	//Starting at the given dirname
	findObject(startdir, input, funnel_type);

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

	cout << "Command: " << sublime_command << endl;

	system(sublime_command.c_str());
}


/*
	Recursively goes through the filesystem from the current directory name
	checking to see if the current directory or file matches on of our input names
*/
void findObject(const char *name, vector<Object> &input, string funnel_type) {

	DIR *dir;
	struct dirent *entry;

	//Check to see if the current directory can be opened
	if(!(dir = opendir(name)))
		return;

	//Checks to make sure that the current directory is readable
	if(!(entry = readdir(dir)))
		return;

	// Do until there is nothing left to read
	do {

		char path[1024];
		int i;
		int len = snprintf(path, sizeof(path) - 1, "%s/%s", name, entry->d_name);
		path[len] = 0;

		// Skip over any names of . or ..
		if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
				continue;

		// Checks to see if one of our input objects matches the current entry
		i = vectorContains(input, string(entry->d_name));

		// If the vectorContains function comes back with a match add the 
		// entry name to the path list of the object
		if (i >= 0) {

			//Gets rid of extra '/' when starting from /
			if (path[0] == '/' && path[1] == '/') {
				memcpy(path, &path[1], sizeof(path) - 1);
			}

			if (funnelCheck(entry, funnel_type)) {
				input.at(i).addPath(string(path));
			}
		}

		//If the entry type is Directory we need to recursively go into it
		if(entry->d_type == DT_DIR) {

			findObject(path, input, funnel_type);
		}

	} while (entry = readdir(dir));

	closedir(dir);
}