
#include "Directory.hpp"

Directory::Directory(const std::string &path) : path_(path), dir_(NULL)
{
}

Directory::~Directory()
{
	if (dir_ != NULL)
	{
		closedir(dir_);
	}
}

bool Directory::open()
{
	dir_ = opendir(path_.c_str());
	if (dir_ == NULL)
	{
		return false;
	}
	return true;
}

std::vector<std::string> Directory::listFiles()
{
	std::vector<std::string> files;
	if (dir_ == NULL && !open())
	{
		return files;
	}
	struct dirent *entry;
	while ((entry = readdir(dir_)) != NULL)
	{
		files.push_back(entry->d_name);
	}
	return files;
}

std::vector<std::string> Directory::listFiles(const std::string& path)
{
	std::vector<std::string> files;
	DIR*	dir = opendir(path.c_str());

	if (dir != NULL)
	{
		struct dirent *entry;
		while ((entry = readdir(dir)) != NULL)
		{
			files.push_back(entry->d_name);
		}
		closedir(dir);
	}
	return files;
}

std::vector<std::pair<std::string,struct stat> >	Directory::listAllDir()
{
	std::vector<std::pair<std::string,struct stat> > files;
	if (dir_ == NULL && !open()) {
		return files;
	}
	struct dirent *entry;
	struct stat fileStat;
	while ((entry = readdir(dir_)) != NULL) {
		std::string filepath = path_ + "/" + entry->d_name;
		if (stat(filepath.c_str(), &fileStat) == 0) {
			files.push_back(std::make_pair(entry->d_name,fileStat));
		}
	}
	return files;
}

/*
	Search directory for file in dirpath
	@param check_subdir check subdirectories also
	@returns File path
*/
std::string	Directory::search(const std::string& file_to_find, bool check_subdir)
{
	std::vector<std::string>	filelist = listFiles();
	std::vector<std::string>	subfilelist;
	std::string	filepath;
	std::size_t	i = 0;

	if (!check_subdir)
	{
		if (vecSearch(filelist, file_to_find))
			filepath = path_ + "/" + file_to_find;
	}
	else
	{
		while (i < filelist.size())
		{
			if (isDir(filelist[i]))
			{
				subfilelist = Directory::listFiles(path_ + "/" + filelist[i]);
				if (vecSearch(subfilelist, file_to_find))
				{
					filepath = path_ + "/" + filelist[i] + "/" + file_to_find;
					break;
				}
			}
			else
			{
				if (vecSearch(filelist, file_to_find))
				{
					filepath = path_ + "/" + file_to_find;
					break;
				}
			}
			++i;
		}
	}
	return (filepath);
}


/*
	Search directory for file in dirpath
	@param check_subdir check subdirectories also
	@returns File path
*/
std::string	Directory::search(const std::string& dirpath, const std::string& file_to_find, bool check_subdir)
{
	std::vector<std::string>	filelist = Directory::listFiles(dirpath);
	std::string	filepath;
	std::size_t	i = 0;

	if (!check_subdir && !filelist.empty())
	{
		if (vecSearch(filelist, file_to_find))
			filepath = dirpath + "/" + file_to_find;
	}
	else
	{
		while (i < filelist.size())
		{
			if (isDir(filelist[i]))
			{
				filelist = Directory::listFiles(dirpath + "/" + filelist[i]);
				if (vecSearch(filelist, file_to_find))
				{
					filepath = dirpath + "/" + filelist[i] + "/" + file_to_find;
					break;
				}
			}
			else
			{
				if (vecSearch(filelist, file_to_find))
				{
					filepath = dirpath + "/" + file_to_find;
					break;
				}
			}
			++i;
		}
	}
	return (filepath);
}

bool Directory::changeDirectory(const std::string &newPath)
{
	if (chdir(newPath.c_str()) == -1)
	{
		std::cerr << "Error al cambiar el directorio a '" << newPath << "': " << strerror(errno) << std::endl;
		return false;
	}
	path_ = newPath;
	return true;
}

std::string Directory::getCurrentDirectory() const
{
	return path_;
}

void Directory::close()
{
	if (dir_ != NULL)
	{
		closedir(dir_);
		dir_ = NULL;
	}
}

/*		Checkers     	*/

/*
	@returns true if file indicated by path is a directory
*/
bool	Directory::isDir(const std::string& filepath)
{
	struct stat fileStat;
	std::string	real_path = getRealPath(filepath);

	if (stat(real_path.c_str(), &fileStat) != 0 || !S_ISDIR(fileStat.st_mode))
		return (false);
	return (true);
}

/*
	Returns true if the filepath is a regular file
*/
bool	Directory::isFile(const std::string& filepath)
{
	struct stat fileStat;
	
	if (stat(filepath.c_str(), &fileStat) != 0 || !S_ISREG(fileStat.st_mode))
		return (false);
	return (true);
}

/*
Returns true if the filepath is an executable
*/
bool	Directory::isExecutable(const std::string& filepath)
{
	struct stat fileStat;

	if ((stat(filepath.c_str(), &fileStat) == 0) && (fileStat.st_mode & S_IXUSR) && !isDir(filepath))
			return (true);
	return (false);
}

/*
	Check if filepath exists
*/
bool Directory::pathExists(const std::string& filepath)
{
	bool	exists = false;

	if (!filepath.empty())
	{
		if (access(filepath.c_str(), F_OK) == 0)
			exists = true;
	}
	return (exists);
}

/*
	Check file permissons

	permissons_mask values:
	- R_OK: Test for read permission
	- W_OK: Test for write permission
	- X_OK: Test for execute permission
	- F_OK: Test for existence

	@returns true if the permissions tested are allowed
*/
bool Directory::checkPermissions(const std::string& filepath, int permissions_mask)
{
	bool	permissions = false;

	if (!filepath.empty())
	{
		if (access(filepath.c_str(), permissions_mask) == 0)
			permissions = true;
	}
	return (permissions);
}

bool	Directory::isImage(const std::string& filepath)
{
	return (ContainsSubstr(getFileType(filepath), "image"));
}

std::string	Directory::readImage(const std::string& filepath)
{
	std::ifstream	file(filepath.c_str(), std::ios::in | std::ios::binary);
	std::string		content;

	if (file.is_open())
	{
		file.seekg(0, std::ios::end);
		content.resize(file.tellg());
		file.seekg(0, std::ios::beg);
		file.read(&content[0], content.size());
		file.close();
	}
	return (content);
}

/*
	Reads file and returns files content 
*/
std::string Directory::readFile(const std::string& filepath)
{
	std::ostringstream	contentStream;
	std::ifstream 		file(filepath.c_str(), std::ios::in | std::ios::binary);

	if (file.is_open())
	{
		contentStream << file.rdbuf();
		file.close();
	}
	return (contentStream.str());
}

std::string	Directory::getFileType(const std::string& filepath)
{
	std::size_t	n = filepath.rfind('.');
	std::string	filetype;
	std::string	extension;
	
	if (n != std::string::npos)
	{
		extension = filepath.substr(n);
		filetype = HTTP::MimeTypeIdToString(HTTP::ExtensionToMimeTypeId(extension));
	}
	else
		filetype = HTTP::MimeTypeIdToString(HTTP::MimeUnknown);
	return (filetype);
}

/*
	Writes content to file
	@note Appends to file by default
*/
void	Directory::writeToFile(const std::string& filepath, const std::string& content)
{
	std::ofstream	stream;
	
	stream.open(filepath.c_str(), std::ios::out | std::ios::app);
	stream << content;
	stream.close();
}

/*
	Writes content to file with the permission mask specified
	
	openmode values:
	- app:		seek to the end of stream before each write
	- binary:	open in binary mode
	- in:		open for reading
	- out:		open for writing
	- trunc:	discard the contents of the stream when opening
	- ate:		seek to the end of stream immediately after open
*/
void	Directory::writeToFile(const std::string& filepath, const std::string& content, std::ios::openmode mode)
{
	std::ofstream	stream;
	
	stream.open(filepath.c_str(), mode);
	stream << content;
	stream.close();
}

std::string	Directory::readFD(const int fd)
{
	char		buffer[BUFSIZ];
	int			len = 1;
	std::string	content;

	if (fd != -1)
	{
		/* Read from File descriptor */
		while (len > 0)
		{
			len = read(fd, buffer, BUFSIZ);
			if (len > 0)
			{
				buffer[len] = '\0';
				content.append(buffer);
			}
		}
	}
	return (content);
}

/*
	Get directory from filepath
	@details iterates through filepath string and divides it by slashes until a directory is found
*/
std::string	Directory::getDirectoryPath(const std::string& filepath, bool check_for_existence)
{
	size_t		lastslash = filepath.find_last_of('/');
	std::string	dirpath;

	if (check_for_existence)
	{
		while (lastslash != STRING_NULL)
		{
			dirpath = filepath.substr(0, lastslash);
			if (Directory::isDir(dirpath))
				break;
			lastslash = dirpath.find_last_of('/');
		}
	}
	else
	{
		if (lastslash != STRING_NULL)
			dirpath = filepath.substr(0, lastslash);
	}
	return (dirpath);
}

static std::vector<std::string>	DefaulRootPaths(void)
{
	std::vector<std::string>	vec;

	vec.push_back(WEBSERV_HOME);
	vec.push_back(WEBSERV_HOME DOCROOT);
	vec.push_back(Directory::getEnv("HOME"));
	vec.push_back(Directory::getEnv("PWD"));
	vec.push_back("..");
	vec.push_back("../..");
	vec.push_back("../../..");
	return (vec);
}

/*
	Search for path in current directory or previous ones
*/
std::string	Directory::getRealPath(const std::string& filepath)
{
	std::vector<std::string>	paths = DefaulRootPaths();
	std::string					buffer;
	std::string					fullpath;
	size_t						i = 0;
	Directory					dir(WEBSERV_HOME DOCROOT);
	
	while (i < paths.size())
	{
		buffer = paths[i] + "/" + filepath;
		ReplaceSubString(buffer, "//", "/");
		if (pathExists(buffer))
		{
			fullpath = buffer;
			break;
		}
		++i;
	}
	if (fullpath.empty())
	{
		fullpath = dir.search(filepath, true);
		if (fullpath.empty())
			fullpath = filepath;
	}
	return (fullpath);
}

std::string	Directory::getEnv(const std::string& key)
{
	const char*	val = std::getenv(key.c_str());
	std::string	stringval;

	if (val)
		stringval = val;
	return (stringval);
}

/* 	File Information */

/*
	Returns the last time the file was accessed
*/
std::string	Directory::getLastFileAccess(const std::string& filepath)
{
	Time		time;
	std::string	timestring;
	struct stat fileStat;

	if (stat(filepath.c_str(), &fileStat) == 0)
	{
		time = fileStat.st_atime;
		timestring = time.getDateTimeString();
	}
	return (timestring);
}

/*
	Returns the last time the file was changed
*/
std::string	Directory::getLastChange(const std::string& filepath)
{
	Time		time;
	std::string	timestring;
	struct stat fileStat;
	
	if (stat(filepath.c_str(), &fileStat) == 0)
	{
		time = fileStat.st_ctime;
		timestring = time.getDateTimeString();
	}
	return (timestring);
}

/*
	Returns the last time the file was modified
*/
std::string	Directory::getLastModification(const std::string& filepath)
{
	Time		time;
	std::string	timestring;
	struct stat fileStat;

	if (stat(filepath.c_str(), &fileStat) == 0)
	{
		time = fileStat.st_mtime;
		timestring = time.getDateTimeString();
	}
	return (timestring);
}

/*
	Returns the file size
*/
intmax_t	Directory::getFileSize(const std::string& filepath)
{
	intmax_t	size = 0;
	struct stat fileStat;

	if (stat(filepath.c_str(), &fileStat) == 0)
		size = fileStat.st_size;
	return (size);
}

