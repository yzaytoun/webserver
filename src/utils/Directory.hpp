/*
 * By: pedromar <pedromar@student.42madrid.com>
 * Created on: 2024-12-24
 * Last modified: 2024-12-24
 */

#ifndef DIRECTORY_HPP
# define DIRECTORY_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <filesystem>
#include "utils.h"
#include "HTTPElements.hpp"
#include "macros.h"
#include "Time.hpp"

class Directory
{
public:
	Directory(const std::string &path);
	~Directory();
	
	/* Basic Methods */
	bool 						open();
	void 						close();
	std::vector<std::string>	listFiles();
	std::vector<std::pair<std::string,struct stat> > listAllDir();
	bool 						changeDirectory(const std::string &newPath);
	std::string					getCurrentDirectory() const;
	std::string					search(const std::string& file_to_find, bool check_subdir = false);
	static std::string			search(const std::string& dirpath, const std::string& file_to_find, bool check_subdir);
	
	/* Checkers */
	static bool					pathExists(const std::string& filepath);
	static bool					isDir(const std::string& filepath);
	static bool					isFile(const std::string& filepath);
	static bool					isExecutable(const std::string& filepath);
	static bool					isImage(const std::string& filepath);
	static bool					checkPermissions(const std::string& filepath, int permissions_mask);
	
	/* Tasks */
	static std::string			getDirectoryPath(const std::string& filepath, bool check_for_existence = true);
	static std::string          readFile(const std::string& filepath);
	static std::string			readImage(const std::string& filepath);
	static std::string			readFD(const int fd);
	static void 				writeToFile(const std::string& filepath, const std::string& content);
	static void 				writeToFile(const std::string& filepath, const std::string& content, std::ios::openmode mode);
	static std::string          getFileType(const std::string& filepath);
	static std::string          getRealPath(const std::string& filepath);
	static std::string			getEnv(const std::string& key);
	static std::vector<std::string>	listFiles(const std::string& path);

	/* File Information */
	static std::string			getLastChange(const std::string& filepath);
	static std::string			getLastFileAccess(const std::string& filepath);
	static std::string			getLastModification(const std::string& filepath);
	static intmax_t				getFileSize(const std::string& filepath);

private:
	std::string path_;
	DIR* 		dir_;
};

#endif
