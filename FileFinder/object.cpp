#ifndef OBJECT_CPP
#define OBJECT_CPP

using namespace std;

class Object {
private:
	vector<string> paths;
	string name = "";
public:

	Object();

	Object(string name){
		this->name = name;
	};

	Object(string name, string path) {
		this->name = name;
		this->paths.push_back(path);
	}

	vector<string> getPaths(){
		return paths;
	}

	void addPath(string path){
		this->paths.push_back(path);
	}

	string getName(){
		return name;
	}

	void setName(string name){
		this->name = name;
	}
};

#endif