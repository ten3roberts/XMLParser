#include "XMLParser.h"
#include <vector>
#include "Utility.h"

using namespace Utility;

XMLNode::XMLNode(std::string filename) : m_tag(""), m_content(""), m_children(), m_depth(0), parent(nullptr)
{
	std::ifstream file(FindFile(filename));
	std::string tmp, file_cont = "";
	while (std::getline(file, tmp)) { file_cont += tmp; }
	Load(file_cont);
}


std::string XMLNode::Load(std::string str)
{

	//Removes unnecessary new line and tab characters
	str = strPurgeAll(str, "\n\t");
	std::string org = str;
	//Finds the beginning of the opening tag
	size_t tag_open;

	//Skipping processing instructions
	while (true)
	{
		tag_open = str.find('<');
		if (str[tag_open + 1] == '?')
		{
			str = str.substr(tag_open + 2);
		}
		else
			break;

		if (tag_open == std::string::npos)
			return str;
	}

	str = str.substr(tag_open + 1);
	//str = str.substr(0, str.find('>'));
	//Finds the end of the opening tag
	size_t tag_end = str.find('>');

	//Divides the inside of the tag into several parts. [0] is the tag name. The other optional ones are attributes
	std::vector<std::string> tag_parts = strSplit(str.substr(0, tag_end), ' ', true);
	if (!tag_parts.size())
		return str;
	m_tag = tag_parts[0];
	if (!m_tag.size())
		return "";
	if(m_tag.back() == '/') //Node is empty
		return org.substr(0, tag_open) + str.substr(tag_open + m_tag.size()-3);

	if (tag_parts.size() > 1)
	{
		for (size_t i = 1; i < tag_parts.size(); i++)
		{
			tag_parts[i] = strPurge(tag_parts[i], '"');
			size_t attrib_equals = tag_parts[i].find('=');
			std::string attrib_name = tag_parts[i].substr(0, attrib_equals);
			std::string attrib_value = tag_parts[i].substr(attrib_equals+1);
			m_attributes[attrib_name] = attrib_value;
		}
	}

	//Indicates the beginning of the closing tag
	size_t close_tag = str.find("</" + m_tag + ">");
	if (close_tag == std::string::npos)
		return "";
	//Find the body between the opening and closing tag
	std::string body = str.substr(tag_end + 1, close_tag - tag_end - 1);


	//Find the children inside of the body. Children returns the part after them
	//Breaks the loop when there are no more childs in the body
	while (body.size())
	{
		XMLNode child(this, m_depth+1);
		std::string tmp(child.Load(body));
		if (tmp == body)
		{
			break;
		}
		body = tmp;


		//Checks if the child was valid
		if (!child.getTag().size())
			continue;
		m_children[child.getTag()].push_back(child);
	}
	m_content = body;
	
	return org.substr(0, tag_open) + str.substr(close_tag + m_tag.size() + 3);
}

