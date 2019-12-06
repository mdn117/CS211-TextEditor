#define PDC_DLL_BUILD 1
#define menu_color 1
#define input_window_color 2
#define info_window_color 3
#define ctrl(x)				((x) & 037) //this defines how I will implement control + another character for keyboard shortcuts
#ifdef _WIN32
//Windows includes
#include "curses.h"
#include "panel.h"
#include "curspriv.h"
#else
//Linux / MacOS includes
#include <curses.h>
#endif
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <stack>
#include "PairComparer.hpp"
#include "Trie.h"
#include "Sorts.hpp"
#include "SortingBenchmark.hpp"

using namespace std;

Trie keywords_trie;
void create_trie(Trie& trie)
{
	ifstream input_file;
	string keyword;
	input_file.open("keywords.txt");
	while (input_file.good())
	{
		getline(input_file, keyword);
		trie.addWord(keyword);
	}
	input_file.close();
}

map<string, int> word_freq(vector<string> contents)
{
	map<string, int> word_freq_chart;
	for (auto word : contents)
	{
		if (word == " ")
		{
			continue;
		}
		else
		{
			word_freq_chart[word] += 1;
		}
	}

	return word_freq_chart;
}

string num_to_bin(int num)
{
	stack<char> num_bit_rep;
	string binary_num;
	if (num == 0)
	{
		binary_num.push_back('0');
		return binary_num;
	}
	while (num > 0)
	{
		if ((num & 1) == 1)
		{
			num_bit_rep.push('1');
		}
		else
		{
			num_bit_rep.push('0');
		}
		num = num >> 1;
	}
	while (num_bit_rep.empty() == false)
	{
		binary_num.push_back(num_bit_rep.top());
		num_bit_rep.pop();
	}
	return binary_num;
}

map<string, string> str_to_bin(map<string, int> freq_chart)
{
	priority_queue< pair<string, int>, vector<pair<string, int>>, MaxHeapPairComparer> max_pq{};
	for (auto word : freq_chart)
	{
		max_pq.push(make_pair(word.first, word.second));
	}
	int priority_num = 0;
	string binary_num;
	stack<char> word_bit_rep;
	map<string, string> word_binary_rep;
	while (max_pq.empty() == false)
	{
		word_binary_rep[max_pq.top().first] = num_to_bin(priority_num);
		max_pq.pop();
		priority_num++;
	}
	return word_binary_rep;
}

void sorted_text(vector<string> text)
{
	InsertionSort<string> Sorter;
	Sorter.sort(text);
}

int main(int argc, char* argv[])
{
	create_trie(keywords_trie);
	//test for character_freq function...
	//vector<string> input_screen = { "hello", "it's", "me", "hello", "can", "you", "you", "you", "hello", "hello", "hello", "me", "can", "can", "can"};
	//map<string, int> frequency_chart = word_freq(input_screen);
	//cout << "--------BEFORE BINARY--------- " << endl;
	//for (auto word : frequency_chart)
	//{
	//	cout << word.first << " : " << word.second << endl;
	//}
	////test for str_to_bin function...
	//map<string, string> priority_chart = str_to_bin(frequency_chart);
	//cout << "--------AFTER BINARY--------- " << endl;
	//for (auto word : priority_chart)
	//{
	//	cout << word.first << " : " << word.second << endl;
	//}
	//test for num_to_bin function...
	/*cout << num_to_bin(0);*/
	//test for trie functions...
	//vector<string> matches = keywords_trie.search("sta");
	//for (int i = 0; i < matches.size(); i++)
	//{
	//	cout << matches[i] << endl;
	//}
	//initialize our screen
	initscr();
	//turn off keyboard echo, better to keep it off when user input is implemented
	noecho();
	//turn on character break, which helps with reading input one character at a time
	cbreak();
	//initializes color to be implemented
	start_color();

	//input_x and input_y are used to determine cursor placement
	int input_y = 1;
	int input_x = 1;
	int search_y = 1;
	int search_x = 1;
	int info_y = 1;
	int info_x = 48;
	vector<string> matches;

	//each of the following functions create windows, the integers are, from left to right,
	//number of rows, number of columns, starting y position, starting x position
	WINDOW* file_window = newwin(3, 8, 0, 0);
	WINDOW* edit_window = newwin(3, 8, 0, 9);
	WINDOW* search_window = newwin(3, 10, 0, 18);
	WINDOW* view_window = newwin(3, 8, 0, 29);
	WINDOW* options_window = newwin(3, 10, 0, 38);
	WINDOW* help_window = newwin(3, 8, 0, 49);
	WINDOW* info_window = newwin(3, 0, 0, 58);
	WINDOW* input_window = newwin(0, 0, 4, 0);
	WINDOW* return_search = newwin(matches.size() + 1, 0, 4, 0);
	PANEL* return_search_panel = new_panel(return_search);
	hide_panel(return_search_panel);
	
	

	wmove(input_window, input_y, input_x);
	refresh();
	wrefresh(input_window);

	//initializes base colors to create a new color, in this case, gray.
	init_pair(menu_color, COLOR_BLACK, COLOR_WHITE);
	init_pair(input_window_color, COLOR_BLUE, COLOR_WHITE);
	init_pair(info_window_color, COLOR_GREEN, COLOR_BLACK);

	//the following block colors the background of the specified window with the color pair
	//specified in the previous function (init_pair).
	wbkgd(input_window, COLOR_PAIR(input_window_color));
	wbkgd(file_window, COLOR_PAIR(menu_color));
	wbkgd(edit_window, COLOR_PAIR(menu_color));
	wbkgd(search_window, COLOR_PAIR(menu_color));
	wbkgd(view_window, COLOR_PAIR(menu_color));
	wbkgd(options_window, COLOR_PAIR(menu_color));
	wbkgd(help_window, COLOR_PAIR(menu_color));
	wbkgd(info_window, COLOR_PAIR(info_window_color));

	//window border. Integer values relate to the character used for the border lengthwise and widthwise (lines for integer 0)

	box(edit_window, 0, 0);
	box(file_window, 0, 0);
	box(search_window, 0, 0);
	box(view_window, 0, 0);
	box(options_window, 0, 0);
	box(help_window, 0, 0);
	box(info_window, 0, 0);

	//turn on keypad input
	keypad(input_window, TRUE);
	keypad(info_window, TRUE);

	//show the cursor
	curs_set(TRUE);

	//MAIN PROGRAM LOGIC GOES HERE

	//outputs the word File to the file window
	mvwprintw(file_window, 1, 2, "File");

	//outputs the word Edit to the edit window
	mvwprintw(edit_window, 1, 2, "Edit");

	//outputs the word Search to the search window
	mvwprintw(search_window, 1, 2, "Search");

	//outputs the word View to the view window
	mvwprintw(view_window, 1, 2, "View");

	//outputs the word Options to the options window
	mvwprintw(options_window, 1, 2, "Options");

	//outputs the word Help to the help window
	mvwprintw(help_window, 1, 2, "Help");

	//outputs the phrase Q to QUIT to the info window
	mvwprintw(info_window, 1, 6, "ctrl + q to QUIT; ctrl + f to search; ctrl + b to sort");

	//Initializes matrix (2D vector)
	vector < vector <string> > words;
	vector <string> word;
	string single_word;
	InsertionSort<string> Sorter;
	int num_of_col = getmaxx(input_window);
	int num_of_row = 3;



	//refresh tells curses to draw everything, also have to refresh each individual
	//window in order to have the colors, text, and boxes show up in each window.
	refresh();
	wrefresh(file_window);
	wrefresh(edit_window);
	wrefresh(search_window);
	wrefresh(view_window);
	wrefresh(options_window);
	wrefresh(help_window);
	wrefresh(info_window);
	wrefresh(input_window);

	

	//END OF PROGRAM LOGIC GOES HERE
	////Pause for user input
	int input;

	fstream file;
	string s;
	string filename;
	string search_word;
	bool cursor_input_win = true;
	bool continue_program = true;

	while (continue_program) //input helps with the switch statements to pause for user input, capital q is used
												 //as a sentinel in order to break out of the while loop
	{
		//if (input == 'O')
		//{
		//	mvwprintw(info_window, 1, 17, "What file do you want to open?");
		//	wmove(info_window, info_y, info_x);
		//	input = wgetch(info_window);
		//	cursor_input_win = false;
		//}
		//else
		//{
		//	wmove(input_window, input_y, input_x);
		//	input = wgetch(input_window);
		//}
		switch (input = wgetch(input_window))
		{
			//each key case will change the x or y value of the cursor, then use the move function to move the cursor inside the window
		case (KEY_LEFT):
			if (cursor_input_win == false)
			{
				info_x--;
				wmove(info_window, info_y, info_x);
				break;
			}
			else
			{
				input_x--;
				wmove(input_window, input_y, input_x);
				break;
			}
		case (KEY_RIGHT):
			input_x++;
			wmove(input_window, input_y, input_x);
			break;
		case (KEY_UP):
			input_y--;
			wmove(input_window, input_y, input_x);
			break;
		case (KEY_DOWN):
			input_y++;
			wmove(input_window, input_y, input_x);
			break;
		//case (10): //10 is the numeric value for enter in ncurses
		//	if (cursor_input_win == false)
		//	{
		//		input_x = 1;
		//		input_y = 1;
		//		wmove(input_window, input_y, input_x);
		//		file.open(filename);
		//		wclear(input_window);
		//		while (file.good() == true)
		//		{
		//			if (input_x > (num_of_col - s.length()))
		//			{
		//				input_x = 1;
		//				input_y++;
		//				num_of_row++;
		//				words.resize(num_of_row, vector<string>(num_of_col));
		//			}
		//			else
		//			{
		//				input_x += s.length();
		//			}
		//			getline(file, s);
		//			word.push_back(s);
		//			words.push_back(word);
		//		}
		//		// display 2d vector contents
		//		for (int i = 0; i < words.size(); i++)
		//		{
		//			for (int j = 0; j < words[i].size(); j++)
		//			{
		//				waddstr(input_window, words[i][j].c_str());
		//			}
		//		}
		//		file.close();
		//		wmove(input_window, input_y, input_x);
		//		wclear(info_window);
		//		box(info_window, 0, 0);
		//		mvwprintw(info_window, 1, 6, "Q to QUIT");
		//		cursor_input_win = true;
		//		break;
		//	}
			//else
			//{
			//	input_x = 1;
			//	input_y++;
			//	wmove(input_window, input_y, input_x);
			//	break;
			//}
		case (127): //127 is the numeric value for backspace in ncurses
			if (cursor_input_win == false)
			{
				info_x--;
				wmove(info_window, info_y, info_x);
				filename.pop_back();
				wdelch(info_window);
				break;
			}
			else
			{
				if (input_x == 1)
				{
					input_y--;
					input_x = num_of_col - 2;
				}
				else
				{
					input_x--;
				}
				wmove(input_window, input_y, input_x);
				wdelch(input_window);
				break;
			}
		case (ctrl('q')):
			continue_program = false;
			break;
		case (ctrl('b')):
			Sorter.sort(word);
			wclear(input_window);
			input_x = 1;
			input_y = 1;
			wmove(input_window, input_y, input_x);
			for (int i = 0; i < word.size(); i++)
			{
				waddstr(input_window, word[i].c_str());
				waddch(input_window, ' ');
				input_x += word[i].size() + 1;
			}
			wmove(input_window, input_y, input_x);
			break;
		case (ctrl('f')):
			if (search_word.empty() == false)
			{
				vector<string> matches = keywords_trie.search(search_word);
				search_y = input_y;
				search_x = input_x;

				for (int i = 0; i < matches.size(); i++)
				{
					waddstr(return_search, matches[i].c_str());
					search_y++;
				}
				search_word = "";
				show_panel(return_search_panel);
				update_panels();
				doupdate();

			}
			break;
			//default case is pressing a key that has no special function other than outputting that character to the screen
		default:
			//if (cursor_input_win == false)
			//{
			//	if (input == 'O')
			//	{
			//		break;
			//	}
			//	filename.push_back(input);
			//	waddch(info_window, input);
			//	info_x++;
			//	wmove(info_window, info_y, info_x);
			//	break;
			//}
			//else
			//{
				if (input_x == num_of_col - 2)
				{
					input_x = 1;
					input_y++;
				}
				else
				{
					input_x++;
				}
				if (input == ' ')
				{
					word.push_back(single_word);
					single_word = " ";
				}
				else
				{
					single_word.push_back(input);
				}
				search_word.push_back(input);
				waddch(input_window, input); //since cbreak is on, addch is not called automatically from getch, so we have to do it each time
				wmove(input_window, input_y, input_x);
				break;
			//}
		}
		wrefresh(input_window);
		
	}

	//end curses mode
	endwin();

	return 0;

}
