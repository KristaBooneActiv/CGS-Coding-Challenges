#pragma once

#include <iostream>
#include <fstream>
#include <iterator>
#include <set>
#include <string>

class Utility
{
public:
	static std::set<int> WriteHighScore(int score)
	{
		// see if file exists and read the values
		std::string fileName = "highscores.txt";
		std::ifstream highScoreFile(fileName);
		std::istream_iterator<int> start(highScoreFile), end;
		std::set<int> highscores(start, end);
		highScoreFile.close();

		// Populate and save if empty
		if (highscores.size() == 0)
		{
			highscores.insert(100);
			highscores.insert(50);
			highscores.insert(20);
			highscores.insert(10);
			highscores.insert(5);
			std::ofstream outfile(fileName);

			std::ostream_iterator<int> output_iterator(outfile, "\n");
			std::copy(highscores.begin(), highscores.end(), output_iterator);

			outfile.close();
		}

		// write player score
		highscores.insert(score);
		// erase lowest score
		highscores.erase(highscores.begin());

		// write newest highscores
		std::ofstream outfile(fileName);
		std::ostream_iterator<int> output_iterator(outfile, "\n");
		std::copy(highscores.begin(), highscores.end(), output_iterator);
		outfile.close();

		return highscores;
	}
};
