#pragma once

#include "ofMain.h"
#include "experimentGovernor.h"
#include "logger.h"
#include "myUtils.h"
#include "telephoneRewiredSettings.h"
#include "telephoneRewiredLoggerCodes.h"


class testApp : public ofBaseApp{
	public:

		TelephoneRewiredSettings settings;

		// Player to show stimuli
		StimulusPlayer stimulusPlayer;
		// Player to show instructions
		InstructionsPlayer instructionsPlayer;
		// State machine to step through the experiment
		ExperimentGovernor experimentGovernor;
		TimedPagePlayer timedPagePlayer;

		int nInstructionPages;
		string participantCode;
		vector< string > subjectiveWords;
		vector< string > subjectiveAssessments;
		int nSubjectiveAssessments;
		string currentSubjectiveAssessment;

		// Data Logger
		LoggerThread logger;
		string logDirPath;
		int vLogFormat;

		KeyStateTracker keyTracker;

		// Variables to control output functionality
		bool showInstructions;
		bool showStimuli;
		bool checkButtonPresses;
		bool logData;

		void stimulusPlay(Stimulus & stimulus);
		void stimulusStop(Stimulus & stimulus);
		void newExperimentState(string & state);
		void newParticipant(unsigned long & participantID);
		void newInstructionsPage(int & pageNumber);
		void buttonDown();
		void buttonUp();

		void setup();
		void update();
		void draw();
		void exit();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void drawInstructionsPage(int & pageNum);
		void drawTimedPage(int & pageNum);
};
