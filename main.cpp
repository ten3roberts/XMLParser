#include "XMLParser.h"
#include "Utility.h"

int main()
{
	//std::string text = ReadFile("Test.xml");
	std::string text = ReadFile("Example.dae");

	XMLNode node;
	node.Load(text);
	node.Save("./Test2.xml");

	for (int i = 0; i < node.getChildren().size(); i++)
	{
		XMLNode* child = node[i];
		printf("Child : %s, content : %s, children : %llu\n", child->getTag().c_str(), child->getContent().c_str(), child->getChildren().size());
	}
	//system("pause");
}