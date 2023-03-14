//Directory recursive deleting programm
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

using namespace std;

void dir_walker(const char *dir)
{
	//Check directory descriptor
	DIR* dir_descriptor =  opendir(dir);
	if(!dir_descriptor)
	{
		cout<<"Unable to open directory - "<<dir<<endl;
		return;
	}

	//Check able of read directory
	dirent* dir_record = readdir(dir_descriptor);
	if(!dir_record)
	{
		cout<<"Unable to read directory - "<<dir<<endl;
                return;
	}

	while(dir_record)
	{
		const string record_name = string(dir) + "/" + string(dir_record->d_name);

		if(string(dir_record->d_name) == "." || string(dir_record->d_name) == "..")
		{
			dir_record = readdir(dir_descriptor);
			continue;
		}

		struct stat buf;

		lstat(record_name.c_str(), &buf);


		if(S_ISDIR(buf.st_mode))
		{
			dir_walker(record_name.c_str());
			rmdir(record_name.c_str());
		}else{
			unlink(record_name.c_str());
		}


		dir_record = readdir(dir_descriptor);
	}
}

int main(int argc, char* argv[])
{
	//Check quantity of arguments
	if(argc < 2)
	{
		cout<<"Input directory for remove"<<endl;
		return -1;
	}

	dir_walker(argv[1]);
	return 0;	

}
