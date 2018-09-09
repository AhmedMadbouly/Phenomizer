#ifndef _PHONEMIZER_H_
#define _PHONEMIZER_H_

#include "QFCommon.h"


//Class to convert Arabic text into phonemes sequence
class Phonemizer
{

public:

	Phonemizer();

	//Convert input arabic text into phonemes sequence
	//Assumes the input is fully diacretized and conform to Quraan writing of AL Muhaddeth
	vstring TextToPhonemes(string text);

	virtual ~Phonemizer() {}

	bool OpenStart() const {return openStart;}
	void OpenStart(bool  value) {openStart = value;}

	bool OpenEnd() const {return openEnd;}
	void OpenEnd(bool  value) {openEnd = value;}

protected:

	bool openStart;
	bool openEnd;

	bool addAllStopOptions;

	CharStrMap letterPhonemeMap;

};


#endif
