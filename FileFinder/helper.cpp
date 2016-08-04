#ifndef HELPER_CPP
#define HELPER_CPP

/*
	Loops through the users input and creates objects
	for the requested file or folder names.
*/
void copyInput(vector<Object> &input, char *argv[], int i) {

	while(argv[i] != "" && argv[i] != 0) {
		Object temp = Object(string(argv[i]));
		input.push_back(temp);
		i++;
	}
}

/*
	Asks the user which file they would like to open 
	if multiple with the same name were found.
*/
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

/*
	If the user specifies the additional command --root the program
	will start from the root directory. Otherwise the program starts
	at the HOME directory by default.

	e.x.:		mysubl --root finder.cpp   	(starts from root)

				mysubl finder.cpp    		(starts from HOME)
*/
int getStartingDir(const char **homedir, char ** argv) {

	int j = 1;

	// Loop through each argument looking for '--root'
	while(argv[j] != "" && argv[j] != 0) {

		// Case 1: Check to see if a --root argument was given
		// Case 2: No '-' found, break out (for performance)
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

	// Will attempt to grab the HOME environment variable of the user
	// If the HOME dir is not set get whatever they have set as their home dir
	if((*homedir = getenv("HOME")) == NULL) {
		*homedir = getpwuid(getuid())->pw_dir;
	}

	return j;
}


/*
	Adds smart "analytics" to our search algorithm. For now it is just a 
	substring analyzer.

	If we have time add a spelling 'analyzer' so if the user misspelt
	a word we can suggest file / directories that are similar

			e.x.: 	fimder ----> finder
*/
bool nameCompare(string input, string search_name) {

	int search_index;
	int input_index;

	// Firstly we need to see if input is a substring of our search_name
	for (search_index = 0, input_index = 0; search_index <= search_name.length(); search_index++) {

		// Case 1: If j reaches the input.length() then input is a substring of search_name.
		// Case 2: Had a bug: if i and j were at their max the for loop broke before Case 1 could hit. Case 2 was implemented
		//		   so that the for loop could go 1 more time, but still break out before an out of bounds error.
		if (input_index == input.length()) {
			return true;
		}
		else if (search_index == search_name.length()) {
			break;
		}

		// Case 1: If the letters do not match reset j and try searching again
		// Case 2: The letters match, move onto the next one
		if (tolower(input.at(input_index)) != tolower(search_name.at(search_index))) {
			input_index = 0;
		}
		else {
			input_index++;
		}
	}


	// A substring of search_name was not found using input.
	return false;
}

/*
	Checks to see if the object vector we have contains the
	current file/directory name during our search.
*/
int vectorContains(vector<Object> input, string name) {

	// Loop through each input object and compare to the
	// file/directory name. Return the index if there is a match.
	//cout << input.size() << endl;
	for (int i = 0; i < input.size(); i++) {

		//if (input.at(i).getName() == name) {
		//cout << input.at(i).getName() << endl;
		if (nameCompare(input.at(i).getName(), name)) {
			return i;
		}
	}

	// Return a -1 if not
	return -1;
}

#endif