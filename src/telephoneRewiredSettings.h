//
//  settings.h
//
//  Created by Sean Montgomery on 12/18/12.
//  http://produceconsumerobot.com/
//
//  This work is licensed under the Creative Commons 
//  Attribution-ShareAlike 3.0 Unported License. 
//  To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/.
//

#ifndef _TELEPHONE_REWIRED_SETTINGS
#define _TELEPHONE_REWIRED_SETTINGS

#include <string>
#include <vector>
//#include "telephoneRewired.h"

class TelephoneRewiredSettings 
{
public:
	string ledArduinoPort;
	string inputArduinoPort;
	string zeoPort;
	int midiPort;
	string logDirPath;

	// Variables to control output functionality
	bool showInstructions;
	bool showStimuli;
	bool checkButtonPresses;
	bool showOscilloscope;
	bool showScreenEntrainment;
	bool showLedEntrainment;
	bool playMidi;
	bool logData;
	bool readEEG;

	// Experiment Timing Variables
	float stimulusOnTime;		// Seconds
	float interStimulusBaseDelayTime;	// Seconds
	float interStimulusRandDelayTime;	// Seconds
	float instructionsTimeoutDelay;		// Seconds
	float congratulationsTime;	//Seconds
	float experimentTimeoutDelay;	//Seconds

	int nInstructionPages;

	TelephoneRewiredSettings() 
	{ 

		// Arduino for outputing on LEDs
		ledArduinoPort = "\\\\.\\COM7"; // Sean, Windows, Uno
		//string ledArduinoPort = "tty.usbmodem1411"; //Mac

		// Arduino for taking button press inputs
		inputArduinoPort = "\\\\.\\COM8";

		// Zeo Port
		zeoPort = "\\\\.\\COM5"; // PC
		//string zeoPort = "tty.usbserial"; //Mac

		// Midi Port
		//int midiPort = 0; // Mac
		midiPort = 1; // PC

		// Log Directory
		logDirPath = "../../LogData/";

		// Variables to control output functionality
		checkButtonPresses = true; // requires Arduino
		showInstructions = true;
		showStimuli = true;

		showScreenEntrainment = false;
		showLedEntrainment = true; // requires Arduino
		playMidi = true;

		readEEG = false; // requires Zeo
		showOscilloscope = false; // sloooows down screen drawing
		logData = true;

		// Experiment Timing Variables
		stimulusOnTime =				.3;		// Seconds
		interStimulusBaseDelayTime =	0.2;		// Seconds
		interStimulusRandDelayTime =	0.1;	// Seconds
		instructionsTimeoutDelay =		30.;	// Seconds
		congratulationsTime =			15.;	// Seconds
		experimentTimeoutDelay =		30.;

		//nInstructionPages = 4;
	}
};

#endif