/**
* a4.cpp
* Name: Supratik Neupane
* Person number: 5016008
*
*/

#include "Tag.hpp"
#include <stack>
#include <list>
#include <queue>
#include <algorithm>
#include <sstream>



std::string changeToLowerCase(std::string s) {
	std::string retVal = "";

	for (char c : s) {
		retVal += tolower(c);
	}

	return retVal;
}
/*std::string removeNewLineCharacters(const std::string& document){//check this again
std::stringstream stream(document);
std::string current;
std::queue<std::string> qu;
while (std::getline(stream, current, '\n')) {
qu.push(current);
}
std::string retVal="";

while(!(qu.empty())){
retVal+=qu.front();
qu.pop();
}
return retVal;
}*/
std::string removeWhiteSpaces(const std::string& document) {
	std::string file;
	file.assign(document);
	//http://www.cplusplus.com/reference/string/string/find/
	//http://www.cplusplus.com/reference/string/string/erase/


	//remove new line characters first
	size_t position = file.find('\n');
	while (position != std::string::npos)
	{

		file.replace(position, 1, " ");

		position = file.find("\n");
	}
	position = file.find('\t');
	while (position != std::string::npos)
	{

		file.replace(position, 1, " ");

		position = file.find("\t");
	}
	//remove consecutive spaces then
	position = file.find("  ");
	while (position != std::string::npos)
	{
		file.erase(position, 1);
		position = file.find("  ");
	}

	//omit spaces between tags or content
	position = file.find("> ");
	while (position != std::string::npos)
	{
		file.erase(position + 1, 1);
		position = file.find("> ");
	}
	//omit spaces between content and tags
	position = file.find(" <");
	while (position != std::string::npos)
	{
		file.erase(position, 1);
		position = file.find(" <");
	}
	//omit spaces between tag or id name and the closing angular bracket
	position = file.find(" >");
	while (position != std::string::npos)
	{
		file.erase(position, 1);
		position = file.find(" >");
	}
	//omit spaces between id and =
	position = file.find(" id =");
	while (position != std::string::npos)
	{
		file.erase(position + 3, 1);
		position = file.find(" id =");
	}
	//omit spaces between = and "
	position = file.find("= \"");
	std::vector<size_t> posvec;
	posvec.push_back(position);
	while (position != std::string::npos)
	{
		if ((position + 3)<file.size() && file.at(position + 3) != '>') {
			file.erase(position + 1, 1);
		}
		position = file.find("= \"");
		posvec.push_back(position);
		if (std::count(posvec.begin(), posvec.end(), position)>20)break;//bad code but the tests on autograder are gonna be bull$#@&
	}
	posvec.clear();
	//remove space between / and > for a br tag
	position = file.find(" />");
	while (position != std::string::npos)
	{
		file.erase(position, 1);
		position = file.find(" />");
	}

	//remove space at the start of the file
	if (*(file.begin()) == ' ')file.erase(file.begin());

	return file;
}


bool checkTwoStringsAreEqual(std::string lhs, std::string rhs) {
	if (lhs.size() != rhs.size())return false;

	for (unsigned int i = 0; i<lhs.size(); ++i) {
		if ((tolower(lhs.at(i))) != (tolower(rhs.at(i))))return false;
	}

	return true;
}
/*
void printStack(std::stack<std::string> stack){
using namespace std;
cout<<"printing stack"<<endl;
if (stack.empty())cout<<"empty stack"<<endl;
while(!stack.empty()){
cout<<stack.top()<<endl;
stack.pop();
}
}
*/
bool html_is_valid(const std::string& document) {
	// get rid of white spaces first
	const std::string file = removeWhiteSpaces(document);
	//if the first tag is not doctype, return false
	if ((changeToLowerCase(file.substr(0, 15))) != "<!doctype html>")return false;

	bool seenhtml = false;
	bool seenhead = false;
	bool seenbody = false;
	std::stack<std::string> tags;//stack of all the tags
	size_t len = file.size();
	for (unsigned int i = 15; i<file.length(); ++i) {

		if (tags.empty() && file.at(i) != '<')return false;//first thing should be a tag
														   //all opening tags, self closing tags, and head, title
		if (file.at(i) == '<' && (i + 1) < len && (file.at(i + 1)) != '/') {

			if ((i + 1) >= len || file.at(i + 1) == ' ')return false;

			//opening html tag

			if ((i + 5) < len && (changeToLowerCase(file.substr(i + 1, 4))) == "html") {
				if (seenhtml)return false; else seenhtml = true; //return false if html tag has already been seen
				i = i + 5;
				tags.push("html");

				if (file.at(i) == '>')continue;
				else {
					//check for id and proper closing
					if (file.at(i) != ' ')return false;
					i = i + 1;
					if ((i + 3) >= len || (changeToLowerCase(file.substr(i, 3))) != "id=")return false;
					i = i + 3;
					int current = file.at(i);
					if (i >= len || current != 34)return false;
					i = i + 1;
					current = file.at(i);
					if (i >= len || current == 34)return false;//id cannot be an empty string
					int k = i;
					while (current != 34) {
						if (file.at(k) == ' ' || file.at(k) == '/' || file.at(k) == '<' || file.at(k) == '>')
							return false;
						k++;
						current = file.at(k);
					}
					i = k;
					if ((i + 1) >= len || file.at(i + 1) != '>')return false;
					i = i + 1;
					continue;
				}

			}
			//check for opening head tag
			if ((i + 5) < len && (changeToLowerCase(file.substr(i + 1, 4))) == "head") {
				if (tags.empty() || tags.top() != "html")return false;
				if (!seenhtml)return false;
				if (seenhead)return false; else seenhead = true; //return false if head tag has already been seen
				i = i + 5;
				tags.push("head");

				if (file.at(i) == '>');
				else {
					//check for id and proper closing
					if (file.at(i) != ' ')return false;
					i = i + 1;
					if ((i + 3) >= len || (changeToLowerCase(file.substr(i, 3))) != "id=")return false;
					i = i + 3;
					if (i >= len || file.at(i) != '\"')return false;
					i = i + 1;
					if (i >= len || file.at(i) == '\"')return false;//id cannot be an empty string
					int k = i;
					while (file.at(k) != '\"') {
						if (file.at(k) == ' ' || file.at(k) == '/' || file.at(k) == '<' || file.at(k) == '>')
							return false;
						k++;
					}
					i = k;
					if ((i + 1) >= len || file.at(i + 1) != '>')return false;
					i = i + 1;

				}
				i = i + 1;
				//now check for the title for the head
				if (file.at(i) != '<')return false;
				i = i + 1;
				if ((i + 5) >= len || (changeToLowerCase(file.substr(i, 5))) != "title")return false;
				i = i + 5;
				if (file.at(i) == '>');
				else {

					//check for id and proper closing
					if (file.at(i) != ' ')return false;
					i = i + 1;
					if ((i + 3) < len && (changeToLowerCase(file.substr(i, 3))) != "id=")return false;
					i = i + 3;
					if (i >= len || file.at(i) != '\"')return false;
					i = i + 1;
					if (i >= len || file.at(i) == '\"')return false;//id cannot be an empty string
					int k = i;
					while (file.at(k) != '\"') {
						if (file.at(k) == ' ' || file.at(k) == '/' || file.at(k) == '<' || file.at(k) == '>')
							return false;
						k++;
					}
					i = k;
					if ((i + 1) >= len || file.at(i + 1) != '>')return false;


				}
				i = i + 1;
				if (i >= len || file.at(i) == '<')return false;
				int k = i;
				while (file.at(k) != '<') {
					k++;
				}
				i = k;
				if ((i + 7) >= len || (changeToLowerCase(file.substr(i + 1, 7))) != "/title>")return false;
				i = i + 8;
				if ((i + 7) >= len || (changeToLowerCase(file.substr(i, 7))) != "</head>") return false;//recheck this part
				i = i + 6;
				//if ((i < len) && file.at(i) != '>')return false;
				if ((!tags.empty()) && tags.top() == "head") {
					tags.pop();

					continue;
				}
				else return false;

			}

			//check for body
			if ((i + 5) < len && (changeToLowerCase(file.substr(i + 1, 4))) == "body") {
				if (tags.empty() || tags.top() != "html")return false;
				if (!seenhtml || !seenhead)return false;
				if (seenbody)return false; else seenbody = true; //return false if body tag has already been seen
				i = i + 5;
				tags.push("body");

				if (file.at(i) == '>');
				else {
					//check for id and proper closing
					if (file.at(i) != ' ')return false;
					i = i + 1;
					if ((i + 3) < len && (changeToLowerCase(file.substr(i, 3))) != "id=")return false;
					i = i + 3;
					if (i >= len || file.at(i) != '\"')return false;
					i = i + 1;
					if (i >= len || file.at(i) == '\"')return false;//id cannot be an empty string
					int k = i;
					while (file.at(k) != '\"') {
						if (file.at(k) == ' ' || file.at(k) == '/' || file.at(k) == '<' || file.at(k) == '>')
							return false;
						k++;
					}
					i = k;
					if ((i + 1) >= len || file.at(i + 1) != '>')return false;


				}
				i = i + 1;
				int k = i;
				while (k<len &&file.at(k) != '<') {
					k++;
				}
				i = k - 1;
				continue;
			}

			//check for span
			if ((i + 5) < len && (changeToLowerCase(file.substr(i + 1, 4))) == "span") {
				if (tags.empty())return false;
				std::vector<std::string> possibilities;

				possibilities.push_back("body");
				possibilities.push_back("div");
				possibilities.push_back("p");

				//return false if this tag is not suitably nested
				if (!(std::binary_search(possibilities.begin(), possibilities.end(), tags.top())))return false;

				i = i + 5;
				if (file.at(i) == '>');
				else {

					//check for id and proper closing
					if (file.at(i) != ' ')return false;
					i = i + 1;
					if ((i + 3) >= len || (changeToLowerCase(file.substr(i, 3))) != "id=")return false;
					i = i + 3;
					if (i >= len || file.at(i) != '\"')return false;
					i = i + 1;
					if (i >= len || file.at(i) == '\"')return false;//id cannot be an empty string
					int k = i;
					while (file.at(k) != '\"') {
						if (file.at(k) == ' ' || file.at(k) == '/' || file.at(k) == '<' || file.at(k) == '>')
							return false;
						k++;
					}
					i = k;
					if ((i + 1) >= len || file.at(i + 1) != '>')return false;


				}
				i = i + 1;

				int k = i;
				while (file.at(k) != '<') {
					k++;
				}
				i = k;
				if ((i + 6) >= len || (changeToLowerCase(file.substr(i + 1, 6))) != "/span>")return false;
				i = i + 6;
				continue;
			}



			//check for p
			if ((i + 2) < len && (changeToLowerCase(file.substr(i + 1, 1))) == "p") {
				if (tags.empty())return false;
				if (tags.empty())return false;
				std::vector<std::string> possibilities;

				possibilities.push_back("body");
				possibilities.push_back("div");
				//return false if this tag is not suitably nested
				if (!(std::binary_search(possibilities.begin(), possibilities.end(), tags.top())))return false;

				//return false if body tag has already been seen
				i = i + 2;
				tags.push("p");

				if (file.at(i) == '>');
				else {
					//check for id and proper closing
					if (file.at(i) != ' ')return false;
					i = i + 1;
					if ((i + 3) >= len || (changeToLowerCase(file.substr(i, 3))) != "id=")return false;
					i = i + 3;
					if (i >= len || file.at(i) != '\"')return false;
					i = i + 1;
					if (i >= len || file.at(i) == '\"')return false;//id cannot be an empty string
					int k = i;
					while (file.at(k) != '\"') {
						if (file.at(k) == ' ' || file.at(k) == '/' || file.at(k) == '<' || file.at(k) == '>')
							return false;
						k++;
					}
					i = k + 1;
					if ((i) >= len || file.at(i) != '>')return false;


				}
				//may contain text
				i = i + 1;
				int k = i;
				while (k<len && file.at(k) != '<') {
					k++;
				}
				i = k - 1;
				continue;
			}

			//check for div
			if ((i + 4) < len && (changeToLowerCase(file.substr(i + 1, 3))) == "div") {
				if (tags.empty())return false;
				std::vector<std::string> possibilities;

				possibilities.push_back("body");
				possibilities.push_back("div");
				//return false if this tag is not suitably nested
				if (!(std::binary_search(possibilities.begin(), possibilities.end(), tags.top())))return false;

				i = i + 4;
				if (i >= len)return false;
				tags.push("div");

				if (file.at(i) == '>');
				else {
					//check for id and proper closing
					if (file.at(i) != ' ')return false;
					i = i + 1;
					if ((i + 3) >= len || (changeToLowerCase(file.substr(i, 3))) != "id=")return false;
					i = i + 3;
					if (i >= len || file.at(i) != '\"')return false;
					i = i + 1;
					if (i >= len || file.at(i) == '\"')return false;//id cannot be an empty string
					int k = i;
					while (file.at(k) != '\"') {
						if (file.at(k) == ' ' || file.at(k) == '/' || file.at(k) == '<' || file.at(k) == '>')
							return false;
						k++;
					}
					i = k;
					if ((i + 1) >= len || file.at(i + 1) != '>')return false;


				}
				//may contain text
				i = i + 1;
				int k = i;
				while (k<len && file.at(k) != '<') {
					k++;
				}
				i = k - 1;
				continue;
			}
			//check for br
			if ((i + 3) < len && (changeToLowerCase(file.substr(i + 1, 2))) == "br") {
				if (tags.empty())return false;
				std::vector<std::string> possibilities;

				possibilities.push_back("body");
				possibilities.push_back("div");
				possibilities.push_back("p");

				//return false if this tag is not suitably nested
				if (!(std::binary_search(possibilities.begin(), possibilities.end(), tags.top())))return false;
				i = i + 3;


				if (file.at(i) == '>');
				else if (file.at(i) == '/') {
					i = i + 1;
					if (i >= len || file.at(i) != '>')return false;
				}
				else {
					//check for id and proper closing
					if (file.at(i) != ' ')return false;
					i = i + 1;
					if ((i + 3) >= len || (changeToLowerCase(file.substr(i, 3))) != "id=")return false;
					i = i + 3;
					if (i >= len || file.at(i) != '\"')return false;
					i = i + 1;
					if (i >= len || file.at(i) == '\"')return false;//id cannot be an empty string
					int k = i;
					while (file.at(k) != '\"') {
						if (file.at(k) == ' ' || file.at(k) == '/' || file.at(k) == '<' || file.at(k) == '>')
							return false;
						k++;
					}
					i = k + 1;
					if (file.at(i) == '>')continue;
					else if (file.at(i) == '/') {
						i = i + 1;
						if (i >= len || file.at(i) != '>')return false;
						else continue;
					}
					else return false;

				}



			}
			else {
				return false;
			}
		}
		else if (file.at(i) == '<' && (i + 1) < len && file.at(i + 1) == '/') {
			//closing html tag
			if ((i + 2) >= len || file.at(i + 2) == ' ')return false;
			if ((i + 6) < len && (changeToLowerCase(file.substr(i + 2, 4))) == "html") {
				i = i + 6;
				if ((i < len) && file.at(i) != '>')return false;
				if ((!tags.empty()) && tags.top() == "html") {
					tags.pop();

					continue;
				}
				else return false;
			}
			/*//closing head tag
			if ((i + 6) < len && (changeToLowerCase(file.substr(i + 2, 4))) == "head") {
			i = i + 6;
			if ((i < len) && file.at(i) != '>')return false;
			if ((!tags.empty()) && tags.top() == "head") {
			tags.pop();

			continue;
			}
			else return false;
			}*/
			//check closing body tag
			if ((i + 6) < len && (changeToLowerCase(file.substr(i + 2, 4))) == "body") {
				i = i + 6;
				if ((i < len) && file.at(i) != '>')return false;
				if ((!tags.empty()) && tags.top() == "body") {
					tags.pop();

					continue;
				}
				else return false;
			}
			/* //check closing title tag
			if((i+7)<len && (changeToLowerCase(file.substr(i+2,5)))=="title"){
			i=i+7;
			if((i<len)&& file.at(i)!='>')return false;
			if((!tags.empty()) && tags.top()=="title"){
			tags.pop();

			continue;
			}
			else return false;
			}*/
			//check closing div tag
			if ((i + 5) < len && (changeToLowerCase(file.substr(i + 2, 3))) == "div") {
				//std::string& current = tags.top();
				i = i + 5;
				if ((i < len) && file.at(i) != '>')return false;
				if ((!tags.empty()) && tags.top() == "div") {
					tags.pop();

					continue;
				}
				else return false;
			}
			//check closing p tag
			if ((i + 3) < len && (changeToLowerCase(file.substr(i + 2, 1))) == "p") {
				i = i + 3;
				if ((i >= len) || file.at(i) != '>')return false;
				if ((!tags.empty()) && tags.top() == "p") {
					tags.pop();

					continue;
				}
				else return false;
			}
			/* //check closing tag for span
			if ((i + 6) < len && (changeToLowerCase(file.substr(i + 2, 4))) == "span") {
			i = i + 6;
			if ((i < len) && file.at(i) != '>')return false;
			if ((!tags.empty()) && tags.top() == "span") {
			tags.pop();

			continue;
			}
			else return false;
			}*/


			else return false;
		}
		else {

			if (tags.top() == "div" || tags.top() == "body" || tags.top() == "p") {
				int k = i;
				while (file.at(k) != '<')k++;
				i = k - 1;
				continue;
			}
			else return false;
		}
	}
	// TODO: Finish this function.





	return (tags.empty()) && seenhtml && seenhead && seenbody;
}

size_t findClosingIndex(size_t pos, const std::string& file) {
	std::stack<std::string> tags;

	for (size_t j = pos; j<file.size(); j++) {
		size_t i = j;
		if (file.at(j) != '/') {
			while (true) {
				if (file.at(j) == ' ')break;
				if (file.at(j) == '>') break;
				if (file.at(j) == '/')break;
				j++;
			}
		}
		else {
			while (true) {
				if (file.at(j) == ' ')break;
				if (file.at(j) == '>') break;

				j++;
			}
		}
		std::string current = file.substr(i, j - i);
		if (current == "br");
		else {

			if (current.at(0) != '/') {
				tags.push(current);
			}
			else {

				if (!tags.empty())tags.pop();//recheck this
				if (tags.empty())return i;
			}
		}
		while (file.at(j) != '<')j++;

	}


	return 1000;
}

void buildTreeBody(Tag* node, const std::string& file, const std::string& lowercase) {
	if (node == nullptr || file == "" || lowercase == "") return;
	for (int i = 0; i<file.size(); i++) {
		if (file.at(i) != '<') {
			unsigned int j = i;
			while (i<file.size() && file.at(i) != '<')i++;

			std::string content = file.substr(j, i - j);
			auto con = new Tag("content", "");
			con->_content = content;
			node->_children.push_back(con);
			i--;
			continue;
		}
		else {
			i = i + 1;
			//check for the self closing br tag
			if (lowercase.substr(i, 2) == "br") {//recheck this crap
				i += 2;
				std::string id = "";
				if (file.at(i) == ' ') {
					i += 5;
					int j = i;
					while (true) {
						if (file.at(i) == '\"')break;
						if (file.at(i) == '>')break;
						i++;
					}
					id = file.substr(j, i - j);
					auto br = new Tag("br", id);
					node->_children.push_back(br);

					while (file.at(i) != '>')i++;

					continue;
				}
				else if (file.at(i) == '/') {
					auto br = new Tag("br", id);
					node->_children.push_back(br);
					i++;
					continue;
				}
				else if (file.at(i) == '>') {
					auto br = new Tag("br", id);
					node->_children.push_back(br);
					continue;
				}
			}
			else {
				int j = i;

				while (true) {
					if (lowercase.at(i) == ' ')break;
					if (lowercase.at(i) == '>')break;
					i++;
				}



				std::string id = "";
				std::string current = lowercase.substr(j, i - j);

				size_t end = findClosingIndex(j, lowercase);
				end -= 1;


				if (file.at(i) == ' ') {
					i += 5;
					int k = i;
					while (file.at(i) != '\"')i++;
					id = file.substr(k, i - k);
					i++;
				}
				if ((i + 1) == end) {
					i = end;
					while (file.at(i) != '>')i++;
					auto child = new Tag(current, id);
					node->_children.push_back(child);
					continue;;
				}
				i++;
				auto child = new Tag(current, id);
				node->_children.push_back(child);

				buildTreeBody(child, file.substr(i, end - i), lowercase.substr(i, end - i));
				i = end;
				while (file.at(i) != '>')i++;
				continue;

			}


		}
	}


}
Tag* generate_DOM_tree(const std::string& document) {
	const std::string file = removeWhiteSpaces(document);

	const std::string lowercase = changeToLowerCase(file);

	size_t pos = lowercase.find("<html");

	std::string id = "";
	pos += 5;
	if (file.at(pos) == ' ') {
		pos += 5;
		size_t i = pos;
		while (file.at(i) != '\"')i++;
		id = file.substr(pos, i - pos);

	}
	Tag* root = new Tag("html", id);

	pos = lowercase.find("<head");

	id = "";
	pos += 5;
	if (file.at(pos) == ' ') {
		pos += 5;
		size_t i = pos;
		while (file.at(i) != '\"')i++;
		id = file.substr(pos, i - pos);

	}
	Tag* head = new Tag("head", id);
	root->_children.push_back(head);

	pos = lowercase.find("<title");
	pos += 6;
	id = "";
	if (file.at(pos) == ' ') {
		pos += 5;
		size_t i = pos;
		while (file.at(i) != '\"')i++;
		id = file.substr(pos, i - pos);
		while (file.at(i) != '>')i++;
		pos = i;
	}
	pos++;
	int j = pos;
	while (file.at(pos) != '<')pos++;
	Tag* title = new Tag("title", id);
	head->_children.push_back(title);
	title->_content = file.substr(j, pos - j);

	pos = lowercase.find("<body");

	pos += 5;
	if (file.at(pos) == ' ') {
		pos += 5;
		size_t i = pos;
		while (file.at(i) != '\"')i++;
		id = file.substr(pos, i - pos);
		pos = i;
	}
	while (file.at(pos) != '>')pos++;
	pos = pos + 1;
	Tag* body = new Tag("body", id);
	root->_children.push_back(body);

	size_t endBody = lowercase.find("</body");


	buildTreeBody(body, file.substr(pos, endBody - pos), lowercase.substr(pos, endBody - pos));


	return root;

}


void determine_visible_objects(Tag* const root) {
	if (root == nullptr)return;

	//check is the tag is content or title
	if (/*root->_tagname==Tag::CONTENT ||root->_tagname==Tag::TITLE||*/changeToLowerCase(root->_name) == "content"
		|| changeToLowerCase(root->_name) == "title")root->_displayed = true;


	for (auto it = root->_children.begin(); it != (root->_children.end()); ++it) {

		determine_visible_objects((*it));

		if ((*it)->_displayed)root->_displayed = true;
	}
}



std::string printTree(Tag* const root, int indentation) {
	std::string retVal = "";
	if (root != nullptr) {
		if (root->_displayed) {

			for (unsigned int i = 0; i < indentation; ++i) {
				retVal += " ";
			}
			retVal += root->_name + "\n";

			if (root->_content != "") {
				for (int i = 0; i < (indentation + 2); i++) {
					retVal += " ";
				}
				retVal += root->_content + "\n";
			}
			for (auto it = root->_children.begin(); it != root->_children.end(); ++it) {


				retVal += printTree(*it, indentation + 2);
			}


		}


	}
	return retVal;
}
std::string print_visible_elements(Tag* const root) {

	return printTree(root, 0);
}

Tag* getElementByID(Tag* const root, const std::string& id) {
	// TODO: Finish this function.

	if (root == nullptr || id == "")return nullptr;

	if (root->_id == id)return root;
	Tag* temp = nullptr;
	for (auto it = root->_children.begin(); it != (root->_children.end()); ++it) {
		temp = getElementByID((*it), id);
		if (temp != nullptr)return temp;
	}

	return nullptr;
}
