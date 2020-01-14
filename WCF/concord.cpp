#include <cassert>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>
#include <cstring>
#include <algorithm>
#include "books.h"

#define _countof(x) sizeof(x) / sizeof(x[0])

struct mapless {
	bool operator()(const std::string &l, const std::string &r) const
	{
		return l < r;
	}
};

bool is_chapter(std::string &line, std::string &chap)
{
        
	std::regex re("CHAPTER ([0-9]+)");
	std::smatch m;
	if (std::regex_search(line, m, re)) {
                chap = m[1];
                return true;
        } 

        return false;
        
}
bool is_para(std::string &line, std::string &para)
{
	std::regex re("^([0-9]+). ");
	std::smatch m;
	if (std::regex_search(line, m, re)) {
                para = m[1];
                return true;
        } 

        return false;
}
bool is_trivial(char *tok) 
{
        const char *exceptions[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "10", "A", "a", "An", "an", "at", "of", "the", "with",
        "after", "After", "all", "All", "Although","And", "As", "but", "But", "by", "By", "come", "Come", "every", "Every", "for", "For", "From",
        "from", "he", "He", "his", "His", "in", "In","it", "It", "no", "No", "nor", "Nor", "not", "Not", "Of", "on", "On", "one", "One", "Other", 
        "other", "others", "Others", "Other", "our", "Our", "so", "So", "That", "that", "The", "There", "Therefore", "there", "therefore", "They",
        "they", "this", "This", "to", "To", "Unto", "unto", "under", "Under", "we", "We", "wherefore", "Wherefore", "When", "when", "which", "Which",
        "whosoever", "Whosoever", "yet", "Yet", "again", "against", "alike", "2020"};
        for(int i=0; i < _countof(exceptions); i++) {
                if(strlen(tok) < 4)
                        return true;
                if(strcmp(tok, exceptions[i]) == 0)
                        return true;
        }

        return false;
}
void parseline(std::string &line, std::string &chap, std::string &title, std::string & para,
	       std::map<std::string, std::vector<std::string>, mapless> &words)
{
        std::string entry("Chapter ");
        entry += chap;
        entry += ", ";
        entry += title;
        entry += ", p. ";
        entry += para;

	const char *delim = " ,.;)(&*^%#@:'";
	char *tok = std::strtok(const_cast<char *>(line.c_str()), delim);
	while (tok) {

		auto it = words.find(tok);
		if (it != words.end())
			it->second.push_back(entry);

		else if(!is_trivial(tok)) {
			std::vector<std::string> v;
			v.push_back(entry);
			std::string w(tok);
			words.insert(std::make_pair(w, v));
		}
	        tok = std::strtok(NULL, delim);
	}
}


int main()
{
	std::fstream fs("wcf.txt");
	std::map<std::string, std::vector<std::string>, mapless> words;

        std::string chapter = "";
        std::string title = "";
	std::string paragraph = "";
	std::string line = "";
        bool haschap = false;

	for (int i = 0; i < 66; i++) {
		std::vector<std::string> v;
		std::string ch("Chapter 1, Of Holy Scripture, p. 2");
                v.push_back(ch); 
		std::string b(books[i]);
		words.insert(std::make_pair(b, v));
	}

	if (fs.is_open()) {
		while (!fs.eof()) {
			std::getline(fs, line);
                        if(is_chapter(line, chapter)) {
                                std::getline(fs, line);
                                title = line;
                        } else if(is_para(line, paragraph))
                                parseline(line, chapter, title, paragraph, words);
		}
		fs.close();
	}
        for(auto m : words) {
                std::cout << "--------    " << m.first << "  ---------\n";
                for(auto i : m.second)
                        std::cout << i << "\n";

                std::cout << "\n";
        }
}
