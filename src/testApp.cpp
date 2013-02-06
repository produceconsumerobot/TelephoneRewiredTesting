#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	// **** COMPUTER SPECIFIC VARIABLES **** //

	// Log Directory
	logDirPath = settings.logDirPath;

	// **** END COMPUTER SPECIFIC VARIABLES **** //

	vLogFormat = 1; // Log format specifier
	logger.setDirPath(logDirPath);

	// **** OPTIONS **** //
	subjectiveWords.push_back("happy");
	subjectiveWords.push_back("annoyed");
	subjectiveWords.push_back("smart");
	subjectiveWords.push_back("calm");
	subjectiveWords.push_back("bored");
	subjectiveWords.push_back("alert");
	subjectiveWords.push_back("energetic");
	subjectiveWords.push_back("weird");
	subjectiveWords.push_back("disoriented");
	subjectiveWords.push_back("focused");
	subjectiveAssessments = subjectiveWords;
	nSubjectiveAssessments = subjectiveAssessments.size();

	// Variables to control output functionality
	checkButtonPresses = settings.checkButtonPresses; // requires Arduino
	showInstructions = settings.showInstructions; 
	showStimuli = settings.showStimuli; 

	logData = settings.logData; 

	// Experiment Timing Variables
	float stimulusOnTime = settings.stimulusOnTime; // Seconds
	float interStimulusBaseDelayTime = settings.interStimulusBaseDelayTime;	// Seconds
	float interStimulusRandDelayTime =	settings.interStimulusRandDelayTime; // Seconds
	float instructionsTimeoutDelay =	settings.instructionsTimeoutDelay; 	// Seconds
	float congratulationsTime =			settings.congratulationsTime; //Seconds
	float experimentTimeoutDelay =			settings.experimentTimeoutDelay; //Seconds


	// Setup experimentGovernor and Listeners
	experimentGovernor = ExperimentGovernor();
	ofAddListener(experimentGovernor.newState, this, &testApp::newExperimentState);
	ofAddListener(experimentGovernor.newParticipant, this, &testApp::newParticipant);
	experimentGovernor.setCongratulationsTime(congratulationsTime);	
	experimentGovernor.setTimeoutDelay(experimentTimeoutDelay); 
	experimentGovernor.addStimulusPaths("data/stimuli/testing/text/form4.txt", "stimuli/testing/audio/form1/");
	experimentGovernor.addStimulusPaths("data/stimuli/testing/text/form1.txt", "stimuli/testing/audio/form4/");

	// Setup Instruction Player
	if (showInstructions) {
		nInstructionPages = nSubjectiveAssessments + 2;
		instructionsPlayer = InstructionsPlayer(nInstructionPages, instructionsTimeoutDelay);
		
		// TODO: Setup Listeners
		ofAddListener(instructionsPlayer.newPage, this, &testApp::newInstructionsPage);
		ofAddListener(instructionsPlayer.drawPage, this, &testApp::drawInstructionsPage);

		experimentGovernor.setInstructionsPlayer(&instructionsPlayer);
		experimentGovernor.nextState();

		ofAddListener(timedPagePlayer.drawPage, this, &testApp::drawTimedPage);
		experimentGovernor.setTimedPagePlayer(&timedPagePlayer);
	}

	// Setup StimulusPlayer
	if (showStimuli) {
		//stimulusPlayer = StimulusPlayer("data/stimuli/");
		//stimulusPlayer.loadStimuli("data/stimuli/form1.txt", "stimuli/sounds/form4/");
		stimulusPlayer.setTimes(stimulusOnTime, interStimulusBaseDelayTime, interStimulusRandDelayTime);
		stimulusPlayer.setIterators(false, true) ;

		// Setup Listeners
		ofAddListener(stimulusPlayer.stimulusPlay, this, &testApp::stimulusPlay);
		ofAddListener(stimulusPlayer.stimulusStop, this, &testApp::stimulusStop);
		//stimulusPlayer.randomizeStimuli();

		experimentGovernor.setStimulusPlayer(&stimulusPlayer);
	}
	
	// Startup screen display parameters
	ofBackground(0, 0, 0);

		// Sync with screen (~50Hz?)
	//ofSetVerticalSync(true);

		// Start the data logger thread
	if (logData) {
		logger.startThread(true, false);
	}

	ofSetFullscreen(true);	
	ofHideCursor();
}


//--------------------------------------------------------------
// Callback function to show different instructions pages
void testApp::drawInstructionsPage(int & pageNum) {
	if (showInstructions) {
		int i = pageNum;
		if (i == 0)
		{
			ofTrueTypeFont font;
			font.loadFont("verdana.ttf", 15, true, true);
			ofColor fontColor(255,255,255);
			ofPoint stimulusCenter(ofGetWindowWidth()/2, ofGetWindowHeight()/2);

			std::stringstream ss;
			ss << "TELEPHONE REWIRED, is an immersive audiovisual art installation and scientific \n";
			ss << "experiment examining the role of oscillations in the brain and the future of human \n";
			ss << "cognition. The lights and sound inside the room are designed to mimic frequencies \n";
			ss << "ordinarily created by neurons in your brain.  After several minutes of experiencing \n";
			ss << "TELEPHONE REWIRED your neurons will begin to synchronize with the \n";
			ss << "installation, firing in a similar pattern. \n";
			ss << "\n";
			ss << "We invite you to participate in an experiment studying how this installation \n";
			ss << "changes your brainwave EEG oscillations and how it affects your, perception, \n";
			ss << "attention and memory.  This will take about 6 minutes in the adjacent room and 5 \n";
			ss << "more minutes to complete a survey on this computer.\n";
			ss << "\n";
			ss << "If you have not seen the installation yet, please go to the computer inside the room \n";
			ss << "first. If you choose to participate in the experiment, you will receive a code number \n";
			ss << "inside that you can enter here.\n";
			ss << "\n";
			ss << "Please enter your code number to continue: ";
			ss << participantCode;
			string data1 = ss.str();

			ofPushMatrix();
			ofPushStyle();
			ofRectangle bounds1 = font.getStringBoundingBox(data1, 0, 0);
			ofTranslate(-bounds1.width/2, -bounds1.height / 2, 0);
			ofSetColor(fontColor);
			font.drawString(data1, stimulusCenter.x, stimulusCenter.y);
			ofPopStyle();
			ofPopMatrix();
		}
		else if ((i > 0) && (i < (1 + nSubjectiveAssessments)))
		{
			ofTrueTypeFont font;
			font.loadFont("verdana.ttf", 15, true, true);
			ofColor fontColor(255,255,255);
			ofPoint stimulusCenter(ofGetWindowWidth()/2, ofGetWindowHeight()/2);

			std::stringstream ss;
			ss << "Press the number on the keypad that indicates how much you agree with the \n";
			ss << "following statement.\n";
			ss << "\n";
			ss << "\n";
			ss << "\n";
			ss << "The installation made me feel ";
			ss << subjectiveAssessments.at(i-1) << ".";
			ss << "\n";
			ss << "\n";
			ss << "\n";
			ss << "\n";
			ss << "1 = Strongly disagree\n";
			ss << "2 = Disagree\n";
			ss << "3 = Neither agree nor disagree\n";
			ss << "4 = Agree\n";
			ss << "5 = Strongly agree\n";
			string data1 = ss.str();

			ofPushMatrix();
			ofPushStyle();
			ofRectangle bounds1 = font.getStringBoundingBox(data1, 0, 0);
			ofTranslate(-bounds1.width/2, -bounds1.height / 2, 0);
			ofSetColor(fontColor);
			font.drawString(data1, stimulusCenter.x, stimulusCenter.y);
			ofPopStyle();
			ofPopMatrix();
		} else if (i == (1 + nSubjectiveAssessments)) {
			ofTrueTypeFont font;
			font.loadFont("verdana.ttf", 15, true, true);
			ofColor fontColor(220,220,220);
			ofPoint stimulusCenter(ofGetWindowWidth()/2, ofGetWindowHeight()/2);

			std::stringstream ss;
			ss << "Please put on the headphones to take a memory test. \n";
			ss << "\n";
			ss << "During the test, words will appear on-screen or be read aloud\n";
			ss << "through the headphones. \n";
			ss << "\n";
			ss << "If the word was presented to you in the installation room, press 1.\n";
			ss << "If it's a new word that was not presented in the installation room, press 0.\n";
			ss << "Please make your answer as quickly as you can.\n";
			ss << "\n";
			ss << "Please press 1 to continue with the memory test.\n";

			string data1 = ss.str();//"this is page 3";
			ofPushMatrix();
			ofPushStyle();
			ofRectangle bounds1 = font.getStringBoundingBox(data1, 0, 0);
			ofTranslate(-bounds1.width/2, -bounds1.height / 2, 0);
			ofSetColor(fontColor);
			font.drawString(data1, stimulusCenter.x, stimulusCenter.y);
			ofPopStyle();
			ofPopMatrix();
		}
	}
}

//--------------------------------------------------------------
// Callback function to show different timed pages
void testApp::drawTimedPage(int & pageNum) {
	if (showInstructions) {
		switch (pageNum) {
		case TimedPagePlayer::Congratulations:
			{
				ofTrueTypeFont font;
				font.loadFont("verdana.ttf", 15, true, true);
				ofColor fontColor(255,255,255);
				ofPoint stimulusCenter(ofGetWindowWidth()/2, ofGetWindowHeight()/2);

				std::stringstream ss;
				ss << "Thank you for participating. We hope you enjoyed Telephone Rewired. Please visit \n";
				ss << "the artists' websites for more information and future updates.\n";
				ss << "\n";
				ss << "Sean Montgomery - http://www.produceconsumerobot.com/\n";
				ss << "LoVid - http://lovid.org/\n";
				string data1 = ss.str();//"this is page 3";
				//string data1 = "Please press the ";
				//string data2 = "GREEN button to begin";
				ofPushMatrix();
				ofPushStyle();
				ofRectangle bounds1 = font.getStringBoundingBox(data1, 0, 0);
				//ofRectangle bounds2 = font.getStringBoundingBox(data2, 0, 0);
				ofTranslate(-bounds1.width/2, -bounds1.height / 2, 0);
				ofSetColor(fontColor);
				font.drawString(data1, stimulusCenter.x, stimulusCenter.y);
				ofPopStyle();
				ofPopMatrix();
			}
			break;
		case TimedPagePlayer::ThankYou:
			{
				ofTrueTypeFont font;
				font.loadFont("verdana.ttf", 15, true, true);
				ofColor fontColor(0,220,0);
				ofPoint stimulusCenter(ofGetWindowWidth()/2, ofGetWindowHeight()/2);

				std::stringstream ss;
				ss << "Thank You!\n" ;
				string data1 = ss.str();//"this is page 3";
				//string data1 = "Please press the ";
				//string data2 = "GREEN button to begin";
				ofPushMatrix();
				ofPushStyle();
				ofRectangle bounds1 = font.getStringBoundingBox(data1, 0, 0);
				//ofRectangle bounds2 = font.getStringBoundingBox(data2, 0, 0);
				ofTranslate(-bounds1.width/2, bounds1.height / 2, 0);
				ofSetColor(fontColor);
				font.drawString(data1, stimulusCenter.x, stimulusCenter.y);
				ofPopStyle();
				ofPopMatrix();
			}
			break;		
		case TimedPagePlayer::BlankPage:
			{
				ofTrueTypeFont font;
				font.loadFont("verdana.ttf", 15, true, true);
				ofColor fontColor(220,220,220);
				ofPoint stimulusCenter(ofGetWindowWidth()/2, ofGetWindowHeight()/2);

				std::stringstream ss;
				ss << "Loading stimuli using your code number...\n" ;
				
				string data1 = ss.str();//"this is page 3";
				ofPushMatrix();
				ofPushStyle();
				ofRectangle bounds1 = font.getStringBoundingBox(data1, 0, 0);
				ofTranslate(-bounds1.width/2, -bounds1.height / 2, 0);
				ofSetColor(fontColor);
				font.drawString(data1, stimulusCenter.x, stimulusCenter.y);
				ofPopStyle();
				ofPopMatrix();
			}
		default:
			// blank page
			break;
		}
	}
}

//--------------------------------------------------------------
// Callback function to log stimulus ON events
void testApp::stimulusPlay(Stimulus & stimulus) {
	if (logData) {
		if (logger.isThreadRunning()) logger.lock();
		std::stringstream ss;
		ss << myGetElapsedTimeMillis() << "," << vLogFormat << "," << STIMULUS_PLAY_CODE << 
			"," << stimulus.str() << ",\n";
		logger.loggerQueue.push(ss.str());
		//logger.push_back(myGetElapsedTimeMillis(), LoggerData::STIMULUS_PLAY, stimulus.logPrint());
		if (logger.isThreadRunning()) logger.unlock();
	}
}

//--------------------------------------------------------------
// Callback function to log stimulus OFF events
void testApp::stimulusStop(Stimulus & stimulus) {
	if (logData) {
		if (logger.isThreadRunning()) logger.lock();
		std::stringstream ss;
		ss << myGetElapsedTimeMillis() << "," << vLogFormat << ","  << STIMULUS_STOP_CODE << 
			"," << stimulus.str() << ",\n";
		logger.loggerQueue.push(ss.str());
		//logger.push_back(myGetElapsedTimeMillis(), LoggerData::STIMULUS_STOP, stimulus.logPrint());
		if (logger.isThreadRunning()) logger.unlock();
	}
}


//--------------------------------------------------------------
void testApp::newExperimentState(string & state){
#ifdef DEBUG_PRINT 
	printf("newExperimentState()\n");
#endif
	if (logData) {
		logger.lock();
		std::stringstream ss;
		ss << myGetElapsedTimeMillis() << "," << vLogFormat << "," << EXPERIMENT_STATE_CODE << 
			"," << state << ",\n";
		logger.loggerQueue.push(ss.str());
		logger.unlock();
	}
	if (state == ExperimentGovernor::getStateString(ExperimentGovernor::Instructions)) {
		subjectiveAssessments = subjectiveWords;
		ofRandomize(subjectiveAssessments);
		nSubjectiveAssessments = subjectiveAssessments.size();
	}
	if (state == ExperimentGovernor::getStateString(ExperimentGovernor::StimulusPresentation)) {
	}
}

//--------------------------------------------------------------
void testApp::newParticipant(unsigned long & participantID){
#ifdef DEBUG_PRINT 
	printf("newParticipant()\n");
#endif
	if (logData) {
		logger.lock();
		
		std::stringstream ss2;
		ss2 << myGetElapsedTimeMillis() << "," << vLogFormat << "," << PARTICIPANT_NUMBER_CODE << 
			"," << experimentGovernor.reverseParticipantID(participantID) << ",\n";
		logger.loggerQueue.push(ss2.str());

		logger.unlock();
	}
}

//--------------------------------------------------------------
void testApp::newInstructionsPage(int & pageNumber){
#ifdef DEBUG_PRINT 
	printf("newInstructionsPage()\n");
#endif
	/*
	int i = pageNumber;
	if ((i > 0) && (i < (1 + nSubjectiveAssessments))) {
		int randNum = (int) ofRandom(0, subjectiveAssessments.size() - 1);
		currentSubjectiveAssessment = subjectiveAssessments.at(randNum);
		subjectiveAssessments.erase(randNum + subjectiveAssessments.begin());
	}
	*/
	if (logData) {
		logger.lock();
		std::stringstream ss;
		ss << myGetElapsedTimeMillis() << "," << vLogFormat << "," << INSTRUCTIONS_PAGE_CODE <<
			"," << pageNumber << ",\n";
		logger.loggerQueue.push(ss.str());

		std::stringstream ss2;
		ss2 << myGetElapsedTimeMillis() << "," << vLogFormat << "," << SUBJECTIVE_ASSESSMENT_CODE <<
			"," << currentSubjectiveAssessment << ",\n";
		logger.loggerQueue.push(ss2.str());

		logger.unlock();
	}

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	experimentGovernor.update();
	ofSleepMillis(1);


	// Show some 
	if (experimentGovernor.getState() == ExperimentGovernor::StimulusPresentation) {
		std::stringstream ss;
		ss << "0 = new\n";
		ss << "1 = old\n";
		string data1 = ss.str();

		ofTrueTypeFont font;
		font.loadFont("verdana.ttf", 15, true, true);
		ofColor fontColor(220,220,220);
		ofPoint stimulusCenter(ofGetWindowWidth()/2, ofGetWindowHeight()/2+200);

		ofPushMatrix();
		ofPushStyle();
		ofRectangle bounds1 = font.getStringBoundingBox(data1, 0, 0);
		ofTranslate(-bounds1.width/2, -bounds1.height / 2, 0);
		ofSetColor(fontColor);
		font.drawString(data1, stimulusCenter.x, stimulusCenter.y);
		ofPopStyle();
		ofPopMatrix();
	}
	
}

//--------------------------------------------------------------
void testApp::exit(){
#ifdef DEBUG_PRINT 
	printf("exit()\n");
#endif
    logger.waitForThread(true);

}


//--------------------------------------------------------------
void testApp::keyPressed(int key){

	if ((experimentGovernor.getState() == ExperimentGovernor::Instructions) &&
		(instructionsPlayer.getPageNum() == 0))
	{ // We're on the first page waiting for a code to be entered
		if (!keyTracker.getKeyState(((char) key))) { // if the key isn't already down
			if ((key >= 48) && (key <= 57)) { // numbers 0-10
				if (participantCode.size() < 6) {
					participantCode.push_back((char) key);
				}
			}
			if (key == 8) {
				participantCode.pop_back();
			}
			if ((key == 13) && (participantCode.size() == 6)) {
				// log participantID
				if (logData) {

					logger.lock();

					std::stringstream ss;
					ss << myGetElapsedTimeMillis() << "," << vLogFormat << "," << PARTICIPANT_ID_CODE << 
						"," << participantCode << ",\n";
					logger.loggerQueue.push(ss.str());

					logger.unlock();
				}
				participantCode.clear();
				experimentGovernor.buttonPressed();
			}
		}
	} else 	if ((experimentGovernor.getState() == ExperimentGovernor::Instructions) &&
		(instructionsPlayer.getPageNum() > 0))
	{
		if ((((char) key) == '1') || (((char) key) == '2')
			|| (((char) key) == '3')  || (((char) key) == '4')  || (((char) key) == '5'))	
		{
			if (!keyTracker.getKeyState(((char) key))) {
				experimentGovernor.buttonPressed();
			}
		}
	} else {
		if ((((char) key) == '0') || (((char) key) == '1'))	
		{
			if (!keyTracker.getKeyState(((char) key))) {
				experimentGovernor.buttonPressed();
			}
		}
	}
	if (!keyTracker.getKeyState(((char) key))) {
		if (logData) {
			logger.lock();
			std::stringstream ss;
			ss << myGetElapsedTimeMillis() << "," << vLogFormat << "," << KEY_DOWN_CODE << "," <<
				key << ",\n";
			logger.loggerQueue.push(ss.str());
			logger.unlock();
		}
	}

	keyTracker.setKeyState(((char) key), true);
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	//if ((((char) key) == '0') || (((char) key) == '1') || (((char) key) == '2')
	//	|| (((char) key) == '3')  || (((char) key) == '4')  || (((char) key) == '5'))	{
	//		keyTracker.setKeyState(((char) key), false);
	//}
	if (keyTracker.getKeyState(((char) key))) {
		if (logData) {
			logger.lock();
			std::stringstream ss;
			ss << myGetElapsedTimeMillis() << "," << vLogFormat << "," << KEY_UP_CODE << "," <<
				key << ",\n";
			logger.loggerQueue.push(ss.str());
			logger.unlock();
		}
	}

	keyTracker.setKeyState(((char) key), false);

	if ( key == 'f') {
		ofToggleFullscreen();
	}
}


//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}