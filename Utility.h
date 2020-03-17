#pragma once
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <filesystem>
#include <string>
#include <vector>
#include <stdio.h>


static std::string strPurge(const std::string& str, const std::string& keyW)
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

static std::string strPurge(const std::string& str, char chr)
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

static std::string strPurgeAll(const std::string& str, const std::string& pattern)
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

static std::vector<std::string> strSplit(const std::string& str, const std::string& keyW, bool ignore_quotes)
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

static std::vector<std::string> strSplit(const std::string& str, char keyW, bool ignore_quotes)
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

static std::string ReadFile(const std::string& filepath)
{
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
		printf("Unable to open file: %s", filepath);
	}
	file.close();
	return fileCont;
}