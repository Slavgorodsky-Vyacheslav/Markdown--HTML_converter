#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include<vector>

class Exc : public std::exception
{
public:
	Exc(std::string err) :error(err) {};
	std::string get_err() const { return error; }
private:
	std::string error;
};

void read_data_from_file(std::vector<std::string>& strings)
{
	std::cout << "Enter filename to read data" << std::endl;
	std::string filename;
	std::cin >> filename;
	std::ifstream fin(filename);
	if (!fin)
	{
		throw Exc("cant open file for readind");
	}

	while (!fin.eof())
	{
		std::string temp;
		std::getline(fin, temp);
		strings.push_back(temp);
	}
}

void write_to_file(const std::vector<std::string>& strings) 
{
	std::cout << "Enter filename to write data" << std::endl;
	std::string filename;
	std::cin >> filename;
	std::ofstream fout(filename);
	if (!fout)
	{
		throw Exc("cant open file for writing");
	}
	for (auto str : strings)
	{
		fout << str;
	}

}

void find_headind1(std::vector<std::string>& strings)
{
	for (int i = 0; i < strings.size()-1; ++i)
	{
		std::string buffer = strings[i];
		std::string temp = strings[i + 1];

		if (buffer.size() == temp.size() && buffer.size()!=0)
		{
			std::string check;
			for (int i = 0; i < temp.size(); ++i)
			{
				check += "=";
			}
			if (temp == check)
			{
				std::string newstr;
				newstr += "<h1>";
				newstr += buffer;
				newstr += "</h1> \n";
				strings[i] = newstr;
				strings[i + 1].erase();
			}
		}
	}
}

void find_headind2(std::vector<std::string>& strings)
{
	for (int i = 0; i < strings.size(); ++i)
	{
		std::size_t found = strings[i].find("##");
		if (found == std::string::npos)
		{
			continue;
		}
		std::string newstr = strings[i].substr(found + 2);
		newstr.insert(0, "<h2>");
		newstr += ("</h2> \n");
		strings[i] = newstr;
	}

}

void unorderlist(std::vector<std::string>& strings)
{
	for (int i = 0; i < strings.size(); ++i)
	{
		if (strings[i].empty() && strings[i + 1].find("*") != std::string::npos)
		{
			strings[i] += "\n<ul> \n";
			while (strings[i + 1].find("*") != std::string::npos)
			{
				std::size_t found = strings[i + 1].find("*");
				std::string newstr = strings[i + 1].substr(found + 1);
				newstr.insert(0, "<li>");
				newstr += ("</li>");
				strings[i + 1] = newstr + "\n";
				++i;
			}
			strings[i] += "</ul> \n" ;
		}
	}
}

void orderlist(std::vector<std::string>& strings)
{
	for (int i = 0; i < strings.size(); ++i)
	{
		int number = 1;
		if (strings[i].empty() && strings[i + 1].find(std::to_string(number) + ".") != std::string::npos)
		{
			strings[i] += "\n<ol> \n";
			while (strings[i + 1].find(std::to_string(number)) != std::string::npos && !strings[i+1].empty())
			{
				std::size_t found = strings[i + 1].find(std::to_string(number) + ".");
				std::string newstr = strings[i + 1].substr(found + 2);
				newstr.insert(0, "<li>");
				newstr += ("</li>");
				strings[i + 1] = newstr + "\n";
				++i;
				++number;
			}
			strings[i] += "</ol> \n";
		}
	}
}

void bold(std::vector<std::string>& strings)
{
	for (int i = 0; i < strings.size(); ++i)
	{
		std::size_t found1 = strings[i].find("**");
		if (found1 == std::string::npos)
		{
			continue;
		}
		std::size_t found2 = strings[i].find("**", found1 + 2);
		if (found2 == std::string::npos)
		{
			continue;
		}
		std::string newstr = strings[i].substr(0, found1);
		newstr.insert(found1, "<strong>");
		newstr += strings[i].substr(found1+2, found2-2);
		newstr += "</strong>";
		newstr += strings[i].substr(found2 + 2);
		strings[i] = newstr;
	}
}

void horizontalrule(std::vector<std::string>& strings)
{
	for (int i = 0; i < strings.size(); ++i)
	{
		std::size_t found = strings[i].find("---");
		if (found == std::string::npos)
		{
			continue;
		}
		std::string newstr = "<hr/> \n";
		strings[i] = newstr;
	}
}

void paragraphs(std::vector<std::string>& strings)
{
	bool parag = false;
	for (int i = 0; i < strings.size(); ++i)
	{
		std::string newstr;
		if (strings[i].empty())
		{
			parag = !parag;
			if (parag)
			{
				newstr = "\n<p>";
				strings[i] = newstr;
			}	
			else
			{
				newstr = "</p>\n";
				strings[i] = newstr;
			}
		}
	}
}

void linebreak(std::vector<std::string>& strings)
{
	for (int i = 0; i < strings.size(); ++i)
	{
		std::size_t found = strings[i].find("  ");
		if (found == std::string::npos)
		{
			continue;
		}
		std::string newstr = strings[i].substr(0, found);
		newstr += "<br/>\n";
		strings[i] = newstr;
	}
}

void convert(std::vector<std::string>& strings)
{
	find_headind1(strings);
	find_headind2(strings);
	unorderlist(strings);
	orderlist(strings);
	bold(strings);
	paragraphs(strings);
	horizontalrule(strings);
	linebreak(strings);
}

int main()
{
	std::vector<std::string> strings;
	try
	{
		read_data_from_file(strings);
	}
	catch (const Exc& e)
	{
		std::cout << e.get_err() << std::endl;
		return -1;
	}
	convert(strings);
	try
	{
		write_to_file(strings);
	}
	catch (const Exc& e)
	{
		std::cout << e.get_err() << std::endl;
		return -1;
	}
	system("pause");
    return 0;
}