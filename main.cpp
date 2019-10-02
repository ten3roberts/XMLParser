#include "Utility.h"
#include "XMLParser.h"

int main()
{
	std::ifstream file("./Test.xml", std::ios::binary);
	std::string text = Utility::ReadFile("Test.xml");
	//std::string text = Utility::ReadFile("Example.dae");

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