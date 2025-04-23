#include <JuceHeader.h>
#include "Bones/ToneStack.h"


class Mappers
{
	Mappers() = default;
	Mappers(const Mappers&) = delete;
public :
	static void setToneStackBass(recursive_linear_filter::BiquadParams& inBassSetup, const double val) {
		const double sampleRate = 44100; // TODO CHANGE TO REAL SAMPLE RATE, MAKE STATIC FUNCTIONS FR
		const double bassGainDB = 10 * (val - 0.5); // +/- 10
		const double bassFrequency = 400.0;
		const double bassQuality = 0.707;
		recursive_linear_filter::BiquadParams bassParams(sampleRate, bassFrequency, bassQuality, bassGainDB);
		inBassSetup = bassParams;
	}

	static void setToneStackMid(recursive_linear_filter::BiquadParams& inMidSetup, const double val) {

		const double sampleRate = 44100; //TODO CHANGE TO REAL SAMPLE RATE, MAKE STATIC FUNCTIONS FR
		const double midGainDB = 10 * (val - 0.5); // +/- 10
		const double midFrequency = 425.0;
		const double midQuality = midGainDB < 0.0 ? 1.5 : 0.7;
		recursive_linear_filter::BiquadParams midParams(sampleRate, midFrequency, midQuality, midGainDB);
		inMidSetup = midParams;
	}
	static void setToneStackHigh(recursive_linear_filter::BiquadParams& inHighSetup, const double val) 
	{
		const double sampleRate = 44100; // TODO
		const double trebleGainDB = 10 * (val - 0.5); // +/- 10
		const double trebleFrequency = 1800.0;
		const double trebleQuality = 0.707;
		recursive_linear_filter::BiquadParams trebleParams(sampleRate, trebleFrequency, trebleQuality, trebleGainDB);
		inHighSetup = trebleParams;
	}
};
