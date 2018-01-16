#include <iostream>
#include "dto/SQLHelper.h"

int main(const int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "Backup Location not provided. Abort..." << std::endl;
		std::cerr << "Found the following arguments: " << std::endl;
		for (auto i = 1; i < argc; i++)
		{
			std::cerr << " " << i << ": " << argv[i] << std::endl;
		}
		return -1;
	}
	std::cout << "Start DB Backup to " << argv[1] << std::endl;

	dto::SQLHelper sql_helper;
	sql_helper.backup_database(argv[1]);

	std::cout << "Finish DB Backup to " << argv[1] << std::endl;

	return 0;
}
