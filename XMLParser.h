#pragma once
#include <string>
#include <unordered_map>

class XMLNode
{
	std::string m_filepath;
	std::string m_tag;
	std::string m_content;
	XMLNode* parent;
	std::unordered_map<std::string, std::vector<XMLNode>> m_children;
	std::unordered_map<std::string, std::string> m_attributes;
	size_t m_depth;
public:
	XMLNode(std::string filename);
	XMLNode(XMLNode* parent, size_t depth) : m_tag(""), m_content(""), m_children(), m_depth(depth), parent(parent) {}
	XMLNode() : m_tag(""), m_content(""), m_children(), m_depth(0), parent(nullptr) {}
	//Loads the xml part and returns what's left
	std::string Load(std::string str);
	std::string GenerateString();
	//Saves the XML to specified file
	void Save(std::string filepath = "");
	void Save();
	std::string getTag() { return m_tag; }
	std::string getContent() { return m_content; }
	std::unordered_map<std::string, std::vector<XMLNode>>& getChildren() { return m_children; }
	std::vector<XMLNode>& getChildren(std::string tag) { return m_children[tag]; }
	std::vector<XMLNode>& getChildren(size_t index) { auto it = m_children.begin(); std::advance(it, index); return it->second; }
	size_t getDepth() { return m_depth; }

	XMLNode* operator[](std::string tag) { if (m_children.find(tag) != m_children.end()) return &m_children[tag][0]; return nullptr; }
	XMLNode* operator[](size_t index) { auto it = m_children.begin(); std::advance(it, index); return &it->second[0]; }
};


class XMLParser
{
};

