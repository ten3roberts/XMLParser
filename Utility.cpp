#include "Utility.h"

#include <fstream>
#include <filesystem>
#include <Windows.h>

#include <shlobj_core.h>
#include <sys/types.h>
#include <stdio.h>
#include <string>

void LogS(std::string, ...) {}

static std::vector<std::string> s_errorDef = {};
static std::ofstream s_LogSile = std::ofstream();
static std::string s_workingDir = "";

void Utility::setWorkingDir(const std::string& dir)
{
	std::string oldDir = s_workingDir;
	s_workingDir = getPath(dir);
	LogS("Utility", "Working directory changed from \"%s %c %s %c", oldDir, "\" to \"", s_workingDir, "\"");
}

std::string Utility::getWorkingDir()
{
	return s_workingDir;
}

std::string Utility::getAppdata()
{
	TCHAR szPath[MAX_PATH];

	SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA | CSIDL_FLAG_CREATE, NULL, 0, szPath));

	return std::string(szPath) + "\\" + "APPNAME + \\";
}


std::string Utility::Capitalize(const std::string& str)
{
	std::string result(str.size(), ' ');
	for (unsigned int i = 0; i < str.size(); i++)
	{
		result[i] = toupper(str[i]);
	}
	return result;
}

std::string Utility::Uncapitalize(const std::string& str)
{
	std::string result = str;
	for (unsigned int i = 0; i < str.size(); i++)
	{
		result[i] = tolower(str[i]);
	}
	return result;
}

std::string Utility::Title(const std::string& str)
{
	std::string result = str;
	result[0] = toupper(str[0]);
	return result;
}



std::vector<unsigned int> Utility::strFind(const std::string& str, const std::string& keyW)
{
	std::vector<unsigned int> results;

	for (unsigned int i = 0; i < str.size() - (keyW.size() - 1); i++)
	{
		if (str.substr(i, keyW.size()) == keyW)
		{
			results.push_back(i);
		}
	}
	return results;
}

std::vector<std::string> Utility::strSplit(const std::string& str, const std::string& keyW, bool ignore_quotes)
{
	//Terminating string with keyW
	std::string text = str;
	if (!(text.substr(text.size() - keyW.size(), keyW.size()) == keyW))
		text += keyW;

	std::vector<std::string> results;
	bool in_quote = false;
	for (size_t i = 0; i < text.size(); i++)
	{
		if (text[i] == '"' && !(i > 0 && text[i - 1] == '\\'))
			in_quote = !in_quote;

		if (text.substr(i, keyW.size()) == keyW && !(in_quote && ignore_quotes)) //Cursor att keyword
		{
			results.push_back(text.substr(0, i)); //Push left bit to results
			text = text.substr(i + 1); //And remove it from string and reset cursor
			i = 0;
		}
	}

	return results;
}

std::vector<std::string> Utility::strSplit(const std::string& str, char keyW, bool ignore_quotes)
{
	//Terminating string with keyW
	std::string text = str;
	if (!(text.back() == keyW))
		text += keyW;

	std::vector<std::string> results;
	bool in_quote = false;
	for (size_t i = 0; i < text.size(); i++)
	{
		if (text[i] == '"' && !(i > 0 && text[i - 1] == '\\'))
			in_quote = !in_quote;
		
		if (text[i] == keyW && !(in_quote && ignore_quotes)) //Cursor att keyword
		{
			results.push_back(text.substr(0, i)); //Push left bit to results
			text = text.substr(i + 1); //And remove it from string and reset cursor
			i = 0;
		}
	}

	return results;
}

std::string Utility::strClamp(const std::string& str, unsigned int size)
{
	return str.substr(0, size);
}

std::string Utility::strStop(const std::string& str, std::string keyW)
{
	return str.substr(str.find_first_of(keyW));
}

std::string Utility::strPurge(const std::string& str, const std::string& keyW)
{
	std::string result;
	//Left part iterator
	size_t c = 0;
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str.substr(i, keyW.size()) == keyW) //Adding left part of string
		{
			result += str.substr(c, i - c);

			//Skipping past keyW
			i += keyW.size();

			//Cathing up with left iterator
			c = i;
		}
	}
	return result + str.substr(c);
}

std::string Utility::strPurge(const std::string& str, char chr)
{
	std::string result;
	//Left part iterator
	size_t c = 0;
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == chr) //Adding left part of string
		{
			result += str.substr(c, i - c);

			//Skipping past chr
			i++;

			//Cathing up with left iterator
			c = i;
		}
	}
	return result + str.substr(c);
}

std::string Utility::strPurgeAll(const std::string& str, const std::string& pattern)
{
	std::string result;
	//Left part iterator
	bool ok = true;
	for (size_t i = 0; i < str.size(); i++)
	{
		for (size_t j = 0; j < pattern.size(); j++)
			if (str[i] == pattern[j])
			{
				ok = false;
				continue;
			}
		if (ok)
			result += str[i];
		ok = true;
	}
	return result;
}

std::string Utility::ListTostring(float* list, int size)
{
	std::string result;
	for (int i = 0; i < size; i++)
	{
		result += std::to_string(list[i]);
		if (i != size - 1)
			result += "\n";
	}
	return result;
}

std::string Utility::ListTostring(std::vector<std::string> list, const std::string& separator)
{
	std::string result;
	for (unsigned int i = 0; i < list.size(); i++)
	{
		result += (list[i]);
		if (i != list.size() - 1) //Putting carriage return on all entries except the last
			result += separator;
	}
	return result;
}


int Utility::ParseTime(const std::string& str)
{
	int seconds = 0;
	std::string text = str;
	text = Uncapitalize(str);

	std::vector<std::string> parts = strSplit(text, " ");

	for (unsigned int i = 1; i < parts.size(); i++)
	{
		//Removes plural 's'
		if (parts[i][parts[i].size() - 1] == 's')
			parts[i].pop_back();


		if (parts[i] == "second")
		{
			seconds += num(parts[i - 1]);
		}

		else if (parts[i] == "minute")
		{
			seconds += num(parts[i - 1]) * 60;
		}

		else if (parts[i] == "hour")
		{
			seconds += num(parts[i - 1]) * 3600;
		}

		else if (parts[i] == "day")
		{
			seconds += num(parts[i - 1]) * 86400;
		}
	}
	return seconds;
}

std::string Utility::FormatTime(int seconds)
{
	int times[4] = { 0, 0, 0, 0 };//Days, Hours, Minutes, Seconds

	const std::string timesDef[] = { "day", "hour", "minute", "second" };

	times[0] = seconds / 86400;
	seconds = seconds % 86400;

	times[1] = seconds / 3600;
	seconds = seconds % 3600;

	times[2] = seconds / 60;
	seconds = seconds % 60;

	times[3] = seconds;


	std::string result;

	//Indicates whetger the gor loop has yet encountered a nonzero time value
	bool beginned = false;
	for (unsigned int i = 0; i < 4; i++)
	{
		if (times[i] > 0)
			beginned = true;

		result += (times[i] > 0 ? std::to_string(times[i]) + " " + timesDef[i] : "") + (times[i] > 1 ? "s" : "");
		if (beginned && i < 3 && times[i + 1] > 0) //Not last filled
			result += ", ";
	}

	return result;
}

std::string Utility::FormatSeconds(float seconds)
{
	if (seconds > 0.1)
		return std::to_string(seconds) + "s";
	else if (seconds > 0.0001)
		return std::to_string(seconds * 1000) + "ms";
	else if (seconds > 0.0000001)
		return std::to_string(seconds * 1000000) + "us";
	else
		return std::to_string(seconds * 1000000000) + "ns";

}

bool Utility::Contains(std::vector<std::string> list, const std::string& item)
{
	for (unsigned int i = 0; i < list.size(); i++)
	{
		if (list[i] == item)
			return true;
	}
	return false;
}

std::string Utility::digits(int value, size_t nDigits)
{
	std::string result = std::to_string(value);
	if (result.size() < nDigits)
	{
		result.insert(0, nDigits - result.size(), '0');
	}
	else if (result.size() > nDigits)
	{
		result = result.substr(0, nDigits);
	}
	return result;
}

bool Utility::IsFile(std::string path)
{
	if (path.back() == '\\')
		return false;

	struct stat s;
	if (stat(path.c_str(), &s) == 0)
	{
		if (s.st_mode & S_IFDIR)
		{
			//it's a directory
			return false;
		}
		else if (s.st_mode & S_IFREG)
		{
			//it's a file
			return true;
		}
		else
		{
			//something else
			return false;
		}
	}
	else
	{
		//error
		return false;
	}
	return false;
}

bool Utility::IsDir(std::string path)
{

	if (path.back() == '\\')
		return true;

	struct stat s;
	if (stat(path.c_str(), &s) == 0)
	{
		if (s.st_mode & S_IFDIR)
		{
			//it's a directory
			return true;
		}
		else if (s.st_mode & S_IFREG)
		{
			//it's a file
			return false;
		}
		else
		{
			//something else
			return false;
		}
	}
	else
	{
		//error
		return false;
	}
	return false;
}

std::vector<std::string> Utility::ListDir(const std::string& directory)
{
	std::vector<std::string> r;
	for (auto& p : std::filesystem::directory_iterator(directory))
		if (p.status().type() == std::filesystem::file_type::regular)
			r.push_back(p.path().string());
		else if (p.status().type() == std::filesystem::file_type::directory)
			r.push_back(p.path().string() + "\\");
	return r;
}

//Will list all sub-directories in specified directory
std::vector<std::string> Utility::ListDirectories(const std::string& directory)
{
	std::vector<std::string> r;
	for (auto& p : std::filesystem::recursive_directory_iterator(directory))
		if (p.status().type() == std::filesystem::file_type::directory)
			r.push_back(p.path().string() + "\\");
	return r;
}

std::vector<std::string> Utility::ListFiles(const std::string& directory)
{
	std::vector<std::string> r;
	for (auto& p : std::filesystem::directory_iterator(directory))
		if (p.status().type() == std::filesystem::file_type::regular)
			r.push_back(p.path().string());
	return r;
}

std::vector<std::string> Utility::ListAllFiles(const std::string& directory)
{
	std::vector<std::string> r;
	for (auto& p : std::filesystem::recursive_directory_iterator(directory))
		if (p.status().type() == std::filesystem::file_type::regular)
			r.push_back(p.path().string());
	return r;
}

std::vector<std::string> Utility::ListAllDirectories(const std::string& directory)
{
	std::vector<std::string> r;
	for (auto& p : std::filesystem::recursive_directory_iterator(directory))
		if (p.status().type() == std::filesystem::file_type::directory)
			r.push_back(p.path().string() + "\\");
	return r;
}

std::vector<std::string> Utility::ListAll(const std::string& directory)
{
	std::vector<std::string> r;
	for (auto& p : std::filesystem::recursive_directory_iterator(directory))
		if (p.status().type() == std::filesystem::file_type::regular)
			r.push_back(p.path().string());
		else if (p.status().type() == std::filesystem::file_type::directory)
			r.push_back(p.path().string() + "\\");
	return r;
}

std::string Utility::FindFile(const std::string& filename, const std::string& directory)
{
	//Makes sure to remove any preceding path to the filename we're searching for
	std::string fname = getFilename(filename, true);

	std::vector<std::string> files = ListAllFiles(directory);
	for (int i = 0; i < files.size(); i++)
	{
		if (getFilename(files[i]) == fname)
		{
			return files[i];
		}
	}
	LogS("FindFile", "No file found with name: %s", filename);
	return "";
}

std::string Utility::FindFile(const std::string& filename, bool useExtension, const std::string& directory)
{
	//Makes sure to remove any preceding path to the filename we're searching for
	std::string fname = getFilename(filename, useExtension);

	std::vector<std::string> files = ListAllFiles(directory);
	for (int i = 0; i < files.size(); i++)
	{
		if (getFilename(files[i], useExtension) == fname)
		{
			return files[i];
		}
	}
	LogS("FindFile", "No file found with name: %s", filename);
	return "";
}

std::string Utility::ReadFile(const std::string& filepath, bool create)
{
	if (create)
		GenerateFile(filepath, "");

	std::ifstream file(filepath);
	std::string line;
	std::string fileCont; //File content

	if (file.is_open())
	{
		int i = 0;
		while (file.good())
		{
			getline(file, line);
			fileCont += line;

			i++;
		}
	}
	else
	{
		LogS("Unable to open file: %s", ShortenString(filepath, 35));
	}
	file.close();
	return fileCont;

}

void Utility::GeneratePath(const std::string& path)
{
	//path is to long
	if (path.size() >= 248)
	{
		Error(1);
		return;
	}

	//Creates all directories leading the last directory
	std::vector<unsigned int> folderAt = strFind(path, "\\");
	for (unsigned int i = 1; i < folderAt.size(); i++)
	{
		std::string tmpPath = path.substr(0, folderAt[i]);
		std::filesystem::create_directory(tmpPath);
	}

	//Creates the last directory
	std::filesystem::create_directory(path);
}

void Utility::GenerateFile(const std::string& path, const std::string& contents, bool append)
{
	//Path is to long
	if (path.size() >= 248)
	{
		Error(1);
		return;
	}

	//Creates all directories leading up to the file
	std::vector<unsigned int> folders = strFind(path, "\\");
	for (unsigned int i = 1; i < folders.size(); i++)
	{
		std::string tmpPath = path.substr(0, folders[i]);
		std::filesystem::create_directory(tmpPath);
		//CreateDirectory(tmpPath.c_str(), nullptr);
	}

	//Creates the file at the end and pushing optional data into it
	std::ofstream fstream(path, append ? std::ios::app : std::ios::trunc);
	fstream.write(contents.c_str(), contents.size());
	fstream.close();

	return;
}

bool Utility::Copy(const std::string& oldPath, const std::string& newPath)
{
	if (IsFile(oldPath))
	{
		std::ifstream oldFile(oldPath, std::ios::binary);
		std::ofstream newFile(newPath, std::ios::binary);
		if (!oldFile.is_open())
		{
			LogS("CopyFile", "Couldn't open file: %s", oldPath);
			return true;
		}
		newFile << oldFile.rdbuf();
		oldFile.close();
		newFile.close();
	}
	else //Copying entire directories
	{
		//Retrieves all subdirs in oldPath recursively
		std::vector<std::string> dirs = ListAllDirectories(oldPath);
		std::vector<std::string> files = ListAllFiles(oldPath);

		//Copies the directory structure
		for (std::string& dir : dirs)
			//Cuts off parent path and adds new parent path
			GeneratePath(strLead(newPath, '\\') + dir.substr(oldPath.size()));

		for (std::string& file : files)
			Copy(file, strLead(newPath, '\\') + file.substr(oldPath.size()));
	}
	return true;
}

bool Utility::Rename(const std::string& oldPath, const std::string& newPath)
{
	return std::rename(oldPath.c_str(), newPath.c_str());
}

std::string Utility::getExtension(const std::string& path)
{
	unsigned int dotIndex = path.find_last_of(".");
	return path.substr(dotIndex == std::string::npos ? path.size() : dotIndex);
}

std::string Utility::getFilename(const std::string& path, bool keepExtension)
{
	size_t slashIndex = path.find_last_of("\\");
	return path.substr(slashIndex == std::string::npos ? 0 : slashIndex + 1, keepExtension ? path.size() : path.find_last_of(".") - slashIndex - 1);
}

std::string Utility::getPath(const std::string& path)
{
	std::string text = path;

	//Check if text specified is already a directory
	if (!IsFile(path))
		return strLead(path, '\\');

	//File
	size_t slashIndex = text.find_last_of("\\");
	text = text.substr(0, slashIndex + 1); //Includeing slash
	if (text.back() != '\\')
	{
		return text;
	}
	return text;
}

std::string Utility::ShortenPath(const std::string& path, int depth, bool omitIndicator)
{
	std::vector<unsigned int> pos = strFind(path, "\\");
	if (depth >= pos.size())
		return path;
	return  (omitIndicator ? "" : "...") + path.substr(pos[pos.size() - depth]);
}

std::string Utility::ShortenString(const std::string& str, unsigned int size, bool omitIndicator)
{
	size -= 3;
	int start = (str.size() - size);
	if (start > 0)
		return (omitIndicator ? "" : "...") + str.substr(start);
	return str;
}

std::string Utility::DirectoryUp(const std::string& path, unsigned int steps)
{
	std::string text = getPath(path);
	std::vector<unsigned int> folders = strFind(text, "\\");
	unsigned int lastFolder = *(folders.end() - steps - 1);
	return text.substr(0, lastFolder);
}

void Utility::Error(unsigned int code)
{
	//Load errordef if its not loaded
	if (s_errorDef.size() == NULL)
		LoadErrorDef();

	std::string explanation = "Undefined Error";

	for (int i = 0; i < s_errorDef.size(); i++)
	{
		if (s_errorDef[i].substr(0, 3) == std::to_string(code) + ": ")
		{
			explanation = s_errorDef[i].substr(3);
			break;
		}
	}


	//Print and prompt the user
	MessageBox(NULL, explanation.c_str(), "Error", MB_ICONWARNING | MB_OK);
	std::cout << "(Error): " << code << "; " << explanation << std::endl;

	return;
}

void Utility::LoadErrorDef()
{
	//s_errorDef = ReadFile(WORKDIR + "Assets\\Errordef.txt");
}

unsigned int Utility::AddError(const std::string& definition, unsigned int code)
{
	if (s_errorDef.size() == NULL)
		LoadErrorDef();

	if (code == APPEND_CODE)
		code = (unsigned int)s_errorDef.size();


	//Check if proposed error already exists, if so return
	if (s_errorDef[0].size() > 0)
		for (auto err : s_errorDef)
		{
			//Check to see if error code already exists
			if (err.substr(0, 3) == code + ": ")
				return code;
			//Check to see if error definition already exists
			if (err.substr(3) == definition)
				return stoi(err.substr(0, 1));
		}

	s_errorDef.push_back(std::to_string(code) + ": " + definition);
	return code;
}

void Utility::SaveErrorDef()
{
	GenerateFile(WORKDIR + "Assets\\Errordef.txt", ListTostring(s_errorDef), false);
}