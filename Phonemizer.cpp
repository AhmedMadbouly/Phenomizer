#include <iostream>
#include "Phonemizer.h"
#include "QuranInfo.h"
#include "Phonemes.h"
using namespace std;

Phonemizer::Phonemizer()
{
	openStart = false;
	openEnd = false;
	addAllStopOptions = false;

	//Prepare letters phonemes map
	for(int i = 0; i < sizeof(lettersPhonemes) / ((sizeof (string)) * 2) ; i++)
		letterPhonemeMap[lettersPhonemes[i*2][0]] = string(lettersPhonemes[i*2+1]);
}

bool IsTashkeel(char ch) {
	return (ch == 'َ' || ch == 'ِ' || ch == 'ُ' || ch == 'ً' || ch == 'ٍ'
			|| ch == 'ٌ' || ch == 'ّ' || ch == 'ْ');
}

string ReplaceWord(string text, string searchWord, string targetWord)
{
	if (searchWord.size() < 1)
		return text;

	bool beginning = true;
	for(int i = 0; i < (int)text.size(); i++)
	{
		int srcNdx = 0;
		int trgNdx = i;
		int wordStart = i;
		bool doReplace = false;
		while(true)
		{
			if (searchWord[srcNdx] == ' ')
			{
				if (beginning || trgNdx == text.size() || text[trgNdx] == ' ')
				{
					srcNdx++;
					if (trgNdx < (int)text.size() && text[trgNdx] == ' ')
					{
						if (trgNdx == i)
							wordStart++;
						trgNdx++;
					}
					if (srcNdx >= (int)searchWord.size())
					{
						doReplace = true;
						break;
					}
				}
				else
					break;
			}

			if (trgNdx >= (int)text.size())
				break;


			//Skip tashkeel!
			while(trgNdx < (int)text.size() && ((IsTashkeel(text[trgNdx]) && !IsTashkeel(searchWord[srcNdx])) || text[trgNdx] == 'ـ') )
			{
				trgNdx++;
			}

			if (srcNdx == 0)
			{
				wordStart = trgNdx;
			}


			if (trgNdx >= (int)text.size() || text[trgNdx] != searchWord[srcNdx])
				break;

			srcNdx++;
			trgNdx++;


			if (srcNdx >= (int)searchWord.size())
			{
				doReplace = true;
				break;
			}

			beginning = false;

		}
		if (doReplace)
		{
			doReplace = false;
			text.replace(wordStart,trgNdx-wordStart,targetWord);
			i = i + targetWord.size() - 1;
		}

		if (beginning && text[i] != ' ')
			beginning = false;

	}

	return text;
}

string ProcessSpecialWords(string text)
{
	for(int i = 0; i < sizeof(specialWords) / sizeof(string) / 2; i++)
	{
		text = ReplaceWord(text, specialWords[i*2], specialWords[i*2+1]);
	}
	return text;
}

bool IsVowel(string p)
{
	return p == "a" || p == "aa" || p == "u" || p == "uu" || p == "i" || p == "ii" || p == "e";
}

vstring Phonemizer::TextToPhonemes(string text)
{
	//Tokenize text
	//Add dummy starts/ends
	//Process

	text = ProcessSpecialWords(text);

	//Add dummy spaces!
	//text = string(3,' ') + AdjustString(text) + string(5,' ');
	text = string(3,' ') + text + string(5,' ');
	

	vstring labels;
	labels.push_back(""); //dummy phoneme

	bool taaMarboota = false;
  bool removeTaaMarboota = false;
	bool lastTanween = false;

	for(int ndx = 0; ndx < (int)text.size(); ndx++)
	{
		char c = text[ndx];

		if (c == ' ') {
      removeTaaMarboota = true;
			continue;
    }

		string lastPhoneme = labels.back();

		//Check special conditions:

		//**** hamzet wasl
		if (c == 'ا' && (text[ndx-1] == ' ' || ((text[ndx-2] == 'و' || text[ndx-2] == 'ف') && text[ndx-3] == ' ')) && !IsTashkeel(text[ndx+1]))
		{
			//Hamzat wasl for sure.
			if (lastPhoneme != "")
			{
				//Not pronounced


				//**** Elteqaa2 Al sakeneen problem
				//Remove previous long vowel (elteqaa2 sakeneen)
				//int lastNdx = labels.size()-1;
				if (lastPhoneme == AlefMad)
					labels.back() = "a";
				else if (lastPhoneme == WowMad)
					labels.back() = "u";
				else if (lastPhoneme == YaaMad)
					labels.back() = "i";

				lastPhoneme = labels.back();

				if (lastPhoneme != "a" && lastPhoneme != "u" && lastPhoneme != "i")
					labels.push_back("i"); //Add kasra to break elteqaa2 Al sakeneen
			}
			else if (openStart)
			{
        //nothing
				//labels.push_back(OptionalHamzatWasl);
			}
			else
			{
				//Pronounced hamzat wasl
				labels.push_back("@");
				labels.push_back(UnkownTashkeel);
			}

			//Check laam shamseyya
			if (text[ndx+1] == 'ل' && !IsTashkeel(text[ndx+2]) && text[ndx+3] == 'ّ')
				ndx++; //Skip this laam

			continue; //Processed!
		}

		//**** taa marboota
		if (c == 'ة')
			taaMarboota = true;

		//**** madd
		if ((c == 'ا' || c == 'ى') && lastPhoneme == "a" && text[ndx+1] != 'ْ')
		{
			labels.back() = AlefMad;
			continue;
		}
		else if (c == 'و' && lastPhoneme == "u" && !IsTashkeel(text[ndx+1]) && text[ndx-1]!=' ')
		{
			labels.back() = WowMad;
			continue;
		}
		else if (c == 'ي' && lastPhoneme == "i" && !IsTashkeel(text[ndx+1]) && text[ndx-1]!=' ')
		{
			labels.back() = YaaMad;
			continue;
		}

		//**** tanween
		if (c == 'ً' || c == 'ٌ' || c == 'ٍ')
		{
			if (c == 'ً')
			{
				labels.push_back("a");
				labels.push_back("n");
				if (text[ndx+1] == 'ا')
					ndx++;
			}
			else if (c == 'ٌ')
			{
				labels.push_back("u");
				labels.push_back("n");
			}
			else
			{
				labels.push_back("i");
				labels.push_back("n");
			}

			lastTanween = true;
			continue;
		}

		//**** shadda
    if (c == 'ّ' && lastPhoneme != "")
		{
			//Check last noon sakena or meem sakena
			if (labels.size() > 1 && !IsVowel(labels[labels.size()-2]) && labels[labels.size()-2] != "")
				labels.erase( labels.end() - 2 );
			labels.push_back(lastPhoneme);
			continue;
		}

		//hamzet madd
		if (c == 'آ')
		{
			labels.push_back("@");

			//Either elteqaa2 sakeneen or hamzet wasl
			if (labels.size() > 3 && !IsVowel(*(labels.end()-1)) && !IsVowel(*(labels.end()-2)) && 
				(*(labels.end()-3) == "aa" || *(labels.end()-3) == "ii" || *(labels.end()-3) == "uu"))
			{
				if (*(labels.end()-3) == "aa")
					*(labels.end()-3) = "a";
				if (*(labels.end()-3) == "ii")
					*(labels.end()-3) = "i";
				if (*(labels.end()-3) == "uu")
					*(labels.end()-3) = "u";
			}

			labels.push_back("aa");
		}
		
		//**** Add normal letters
		CharStrMap::iterator letterIt = letterPhonemeMap.find(c);
		if (letterIt != letterPhonemeMap.end())
		{
			labels.push_back(letterIt->second);
      if (removeTaaMarboota) {
			  taaMarboota = false;
        removeTaaMarboota = false;
      }
			lastTanween = false;
		}

		if (labels.size() > 4 && !IsVowel(*(labels.end()-1)) && !IsVowel(*(labels.end()-2)) && 
			(*(labels.end()-3) == "aa" || *(labels.end()-3) == "ii" || *(labels.end()-3) == "uu")
			&& *(labels.end()-2) != "m" && *(labels.end()-2) != "n"
			)
		{
			//Either elteqaa2 sakeneen or hamzet wasl
			if (*(labels.end()-3) == "aa")
				*(labels.end()-3) = "a";
			if (*(labels.end()-3) == "ii")
				*(labels.end()-3) = "i";
			if (*(labels.end()-3) == "uu")
				*(labels.end()-3) = "u";
		}

		//**** Eqlaab
		if (labels.back() == "b" && lastPhoneme == "n")
			*(labels.end()-2) = "m";

	}

	if (!openEnd)
	{
		//Adjust last tanween
		if (lastTanween && labels.back() == "n")
		{
			if (*(labels.end()-2) == "a")
			{
				//Tanween fat7 (complex)
				labels.pop_back(); //remove noon
				if (taaMarboota || *(labels.end()-2) == "@")
					labels.pop_back(); //No mad alef in this case
				else
					labels.back() = "aa"; //Replace fat7a with madd alef
			}
			else
			{
				labels.pop_back(); //remove noon
				labels.pop_back(); //remove madd
			}
		}

		//Remove tashkeel
		if (labels.back() == "a" || labels.back() == "u" || labels.back() == "i")
			labels.pop_back();

		//Set taa marboota as ha
		if (taaMarboota && labels.back() == "t")
		{
			labels.back() = "h";
		}

	}

	//Remove the dummy phoneme at the beginning
	labels.erase(labels.begin());

	return labels;
}

int main(int argc, char const *argv[])
{
	freopen("input", "r", stdin);
	string str;
	cin >> str;
	Phonemizer p;
	vstring vec = p.TextToPhonemes(str);
	for (int i = 0; i < vec.size(); ++i)
	{
		std::cout << vec.at(i) << " ";
	}
	cout << "\n";
	return 0;
}
