
string lettersPhonemes[] = {
	"ب","b",
	"ت","t",
	"ث","th",
	"ج","j",
	"ح","h~",
	"خ","kh",
	"د","d",
	"ذ","th",
	"ر","r",
	"ز","z",
	"س","s",
	"ش","sh",
	"ص","S~",
	"ض","D~",
	"ط","T~",
	"ظ","TH~",
	"ع","ay",
	"غ","gh",
	"ف","f",
	"ق","q",
	"ك","k",
	"ل","l",
	"م","m",
	"ن","n",
	"ه","h",
	"و","w",
	"ي","y",
	"أ","@",
	"إ","@",
	"ء","@",
	"ئ","@",
	"ؤ","@",
	"ة","t",
	"َ","a",
	"ُ","u",
	"ِ","i",
};
//Other special phonemes!
#define AlefMad "aa"
#define WowMad "uu"
#define YaaMad "ii"
#define UnkownTashkeel "e"
#define OptionalHamzatWasl "@~"


//Special words to be replaced in the original text before phonetization
//Notes:
//1- The left word tashkel in the text word is ommitted unless it's there in the specialWord here.
//2- Spaces at the beginning or the end of the specialWords here means this is a word start or end not necessarily a space is there.
string specialWords[] = {
	"الله", "اللَّاه",
	"الرحمن", "الرَّحْمَان",
	"لِله", "لِلَّاه", //lellaah
	"لَله", "لَلَّاه", //lallaah
	" لله", "لِلَّاه", //lellaah
	"أولئك", "أُلَائِك",
	"ذَلِك", "ذَالِك",
	"هؤلاء", "هَاؤُلَاء",
	" الم ", "أَلِفْ لَامْ مِيم", //7oroof moqatta3a
	" المر ", "أَلِفْ لَامْ مِيم را",
	" المص ", "أَلِفْ لَامْ مِيم صَاد",
	" الر ", "أَلِفْ لَامْ رَا",
	" كهيعص ", "كَافْ هَا يَا عَيْنْ صَادْ",
	" طه ", "طَا هَا",
	" طسم ", "طَا سِين مِّيمْ",
	" طس ", "طَا سِين",
	" يس ", "يَا سِين",
	" ص ", "صَادْ",
	" حم ", "حَا مِيمْ",
	" عسق ", "عَيْن سِين قَافْ",
	" ق ", "قَافْ",
	" ن ", "نُون",
	"لَكِن", "لَاكِن"
};


