#ifndef _QURAN_INFO_H_
#define _QURAN_INFO_H_

#include "QFCommon.h"


//Big class provides various services regarding quran text
//Quraan is stored as a continuous stream of characters (letters, tashkeel, and spaces (no \n)).
//Indexing heirarchies:
//Soras -> Ayas -> Words -> characters
//Aya -> Aya number within sora
//Joz2 -> Ayas
//Page -> Line -> Words
//Page -> Ayas
//Sora -> Name
//Name -> Sora (bs)
//Character -> Word (bs)
//Word -> Aya (bs)
//Aya -> Sora (bs)
//Aya -> Joz2 (bs)
//Word -> Line (bs)
//Line -> Page (bs)
//Aya -> Page (bs)
class QuranInfo
{
public:
	QuranInfo() {}

	//Load quran from the given file. The file should be in Al Muhaddith format
	void LoadQuranTextMuhaddith(string fileName);

	const vint &WordsNdx() const {return wordsNdx;}
	const vint &AyaysWords() const {return ayasWords;}
	const vint &AyasWords() const {return ayasWords;}
	const vint &SorasAyas() const {return sorasAyas;}
	const vstring &SorasNames() const {return sorasNames;}
	const vint &LinesWords() const {return linesWords;}
	const vint &Joz2Ayas() const {return joz2Ayas;}
	const vint &PagesLines() const {return pagesLines;}
	const vint &PagesAyas() const {return pagesAyas;}
	const vint &AyasNumbers() const {return ayasNumbers;}
	const vint &SorasLines() const {return sorasLines;}
	//int GetSoraNdx(string name) const {return soraNameNdx[name];}

	//Return quran text from the start'th character upton before end'th character.
	//The returned string has end - start characters.
	string GetQuranText(int start, int end) const {return quran.substr(start,end-start);}

	virtual ~QuranInfo() {}
protected:

	string quran;
	vint wordsNdx;
	vint ayasWords;
	vint sorasAyas;
	vstring sorasNames;
	vint linesWords;
	vint joz2Ayas;
	vint pagesLines;
	vint pagesAyas;

	vint ayasNumbers;
	vint sorasLines;

	StrIntMap soraNameNdx;
};




//Uility functions

//Returns true if c is a tashkeel character
bool IsTashkeel(char c);

//Return true if the two strings are the same neglecting tashkeel differences.
bool CompareNoTashkeel(string str1, string str2);

//Trim string and remove '@' character
string AdjustString(string input);

#endif
