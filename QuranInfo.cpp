#include "QuranInfo.h"
#include <fstream>
#include <algorithm>
#include <sstream>

//****************************************
//   UtilityFunctions
//****************************************

//Returns true if c is a tashkeel character
bool IsTashkeel(char c)
{
	return string("ًٌٍَُِّْ").find(c) != string::npos;
}

//Return true if the two strings are the same neglecting tashkeel differences.
bool CompareNoTashkeel(string str1, string str2)
{
	int ndx1 = 0, ndx2 = 0;

	while(ndx1 < (int)str1.length() || ndx2 < (int)str2.length())
	{
		while(ndx1 < (int)str1.length() && IsTashkeel(str1[ndx1])) ndx1++;
		while(ndx2 < (int)str2.length() && IsTashkeel(str2[ndx2])) ndx2++;
		if (ndx1 >= (int)str1.length() || ndx2 >= (int)str2.length())
		{
			if (ndx1 >= (int)str1.length() && ndx2 >= (int)str2.length())
				break;
			else
				return false;
		}

		if (str1[ndx1] != str2[ndx2])
			return false;

		ndx1++; ndx2++;
		
	}
	
	return true;
}


//Trim string and remove '@' and 'ـ' characters
string AdjustString(string input)
{
	int ndx = 0;
	//Remove '@'
	//while((pos = input.find('@',ndx)) != input.npos)
	//{
	//	input.erase( input.begin() + pos );
	//	ndx = pos;
	//}
	string::iterator itEnd = input.end();
	itEnd = remove(input.begin(), itEnd,'@');
	itEnd = remove(input.begin(), itEnd,'ـ');
	input.resize(itEnd - input.begin());

	ndx = 0;
	while(ndx < (int)input.length() && (input[ndx] == ' ' || input[ndx] == '\t' || input[ndx] == '\n' || input[ndx] == '\r'))
		ndx++;
	int startNdx = ndx;
	ndx = input.length() - 1;
	while(ndx >= startNdx && (input[ndx] == ' ' || input[ndx] == '\t' || input[ndx] == '\n' || input[ndx] == '\r'))
		ndx--;
	int endNdx = ndx;

	input = input.substr(startNdx, endNdx - startNdx + 1);

	return input;
}


void QuranInfo::LoadQuranTextMuhaddith(string fileName)
{
	ifstream ifs(fileName.c_str());
	if (!ifs) throw FileNotFoundException(fileName);

	char tempChar;

	//string ayaText = "";
	wordsNdx.push_back(0);
	ayasWords.push_back(0);
	linesWords.push_back(0);


	while(ifs.good())
	{
		string line;
		getline(ifs,line,'\n');
		if (!ifs)
			break;
		
		if (line[0] == '*') //comments
		{
			//Comment lines
			if (line[1] == '1')
			{
				//Joz2
				joz2Ayas.push_back(ayasWords.size()-1);
			}
			else if (line[1] == '2')
			{
				//Parse sora number and name
				int soraNumber;
				string soraName;
				stringstream ss(line.substr(3));
				ss>>soraNumber>>soraName;
				if (!ss) continue; //not a new sora
				//ss>>tempChar;
				getline(ss,soraName,'\n');
				soraName = AdjustString(soraName);

				sorasNames.push_back(soraName);
				sorasAyas.push_back(ayasWords.size()-1);
				sorasLines.push_back(linesWords.size()-1);
				soraNameNdx[soraName] = sorasNames.size()-1;

			}
			else if (line[1] == '3')
			{
				//Read page number
				stringstream ss(line.substr(3));
				int pageNumber;
				ss>>tempChar>>pageNumber;
				//Page number
				if (pageNumber > (int)pagesLines.size())
				{
					pagesLines.push_back(linesWords.size()-1);
					pagesAyas.push_back(ayasWords.size()-1);
				}
				
			}
			continue;
		}

		//Add empty line for sora name
		if ((*sorasLines.rbegin()) == linesWords.size()-1)
		{
			//Sora start, so add the line
			linesWords.push_back( wordsNdx.size()-1 ); //Empty line for sora name
		}


		//Aya text
		line = AdjustString(line);

		bool isSpecialBasmala = false;
		if (CompareNoTashkeel(line,"بسم الله الرحمن الرحيم"))
		{
			line = "بِسْمِ اللَّهِ الرَّحْمـَنِ الرَّحِيمِ";
			if (sorasAyas.size() > 1)
				isSpecialBasmala = true;
		}
		
		stringstream words(line);
		while(words.good())
		{
			string word;
			words>>word;
			if (word.size() == 0) continue;
			if (word == "{س}")
			{
				//Nothing to do!
			}
			else if (word[0] == '{')
			{
				//Aya marker
				ayasWords.push_back(wordsNdx.size()-1);
				int ayaNumber;
				stringstream ss(word.substr(1));
				ss>>ayaNumber;
				ayasNumbers.push_back(ayaNumber);				

			}
			else
			{
				quran.append( word + ' ' );
				wordsNdx.push_back(quran.length());
			}
		}

		if (isSpecialBasmala)
		{
			ayasWords.push_back(wordsNdx.size()-1);
			ayasNumbers.push_back(0);
		}

		linesWords.push_back(wordsNdx.size()-1);


	}


	//indecies ends
	joz2Ayas.push_back(ayasWords.size()-1);
	pagesLines.push_back(linesWords.size()-1);
	pagesAyas.push_back(ayasWords.size()-1);
	sorasLines.push_back(linesWords.size()-1);
	sorasAyas.push_back(ayasWords.size()-1);

}
