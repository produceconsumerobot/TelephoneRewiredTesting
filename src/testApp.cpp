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

	nInstructionPages = settings.nInstructionPages;

	// Setup experimentGovernor and Listeners
	experimentGovernor = ExperimentGovernor();
	ofAddListener(experimentGovernor.newState, this, &testApp::newExperimentState);
	ofAddListener(experimentGovernor.newParticipant, this, &testApp::newParticipant);
	experimentGovernor.setCongratulationsTime(congratulationsTime);	
	experimentGovernor.setTimeoutDelay(experimentTimeoutDelay); 
	experimentGovernor.addStimulusPaths("data/stimuli/training/text/form4.txt", "stimuli/training/audio/form1/");
	experimentGovernor.addStimulusPaths("data/stimuli/training/text/form1.txt", "stimuli/training/audio/form4/");

	// Setup Instruction Player
	if (showInstructions) {
		instructionsPlayer = InstructionsPlayer(instructionsTimeoutDelay);
		
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
		stimulusPlayer.loadStimuli("data/stimuli/form1.txt", "stimuli/sounds/form4/");
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

	
}

//--------------------------------------------------------------
// Callback function to show different instructions pages
void testApp::drawInstructionsPage(int & pageNum) {
	if (showInstructions) {
		int i = pageNum;
		switch (i) {
		case 0:
			{
				ofTrueTypeFont font;
				font.loadFont("verdana.ttf", 20, true, true);
				ofColor fontColor(0,220,0);
				ofPoint stimulusCenter(ofGetWindowWidth()/2, ofGetWindowHeight()/2);

				string data1 = "This is cool MAAX";
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

				break;
			}
		case 1:
			{
				ofTrueTypeFont font;
				font.loadFont("verdana.ttf", 20, true, true);
				ofColor fontColor(0,220,0);
				ofPoint stimulusCenter(ofGetWindowWidth()/2, ofGetWindowHeight()/2);

				string data1 = "this is cool page 2";
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
				break;
			}
		case 2:
			{		
				ofTrueTypeFont font;
				font.loadFont("verdana.ttf", 20, true, true);
				ofColor fontColor(0,220,0);
				ofPoint stimulusCenter(ofGetWindowWidth()/2, ofGetWindowHeight()/2);

				std::stringstream ss;
				ss << "this is rad page 3\n" << "USER ID: " << experimentGovernor.getParticipantID();
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
				break;
			}
		default:
			break;
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
				font.loadFont("verdana.ttf", 20, true, true);
				ofColor fontColor(0,220,0);
				ofPoint stimulusCenter(ofGetWindowWidth()/2, ofGetWindowHeight()/2);

				std::stringstream ss;
				ss << "WEEE DOGGY!! YOUR ARE STOKED MAX!\n" << "USER ID: " << experimentGovernor.getParticipantID();
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
		case TimedPagePlayer::ThankYou:
			{
				ofTrueTypeFont font;
				font.loadFont("verdana.ttf", 20, true, true);
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

		std::stringstream ss;
		ss << myGetElapsedTimeMillis() << "," << vLogFormat << "," << PARTICIPANT_ID_CODE << 
			"," << participantID << ",\n";
		logger.loggerQueue.push(ss.str());
		
		//experimentGovernor.reverseParticipantID(participantID);

		//unsigned long participantID = (participantNumber ^ 2999975935);
		//unsigned long participantID = (participantNumber ^ 313717);
		//unsigned long temp = (participantID ^ 313717);

		std::stringstream ss2;
		ss2 << myGetElapsedTimeMillis() << "," << vLogFormat << "," << PARTICIPANT_NUMBER_CODE << 
			"," << participantID << ",\n";
		logger.loggerQueue.push(ss2.str());

		logger.unlock();
	}
}

//--------------------------------------------------------------
void testApp::newInstructionsPage(int & pageNumber){
#ifdef DEBUG_PRINT 
	printf("newInstructionsPage()\n");
#endif
	if (logData) {
		logger.lock();
		std::stringstream ss;
		ss << myGetElapsedTimeMillis() << "," << vLogFormat << "," << INSTRUCTIONS_PAGE_CODE <<
			"," << pageNumber << ",\n";
		logger.loggerQueue.push(ss.str());
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
	if ((((char) key) == '0') || (((char) key) == '1') || (((char) key) == '2')
		 || (((char) key) == '3')  || (((char) key) == '4')  || (((char) key) == '5'))	{
		if (!keyTracker.getKeyState(((char) key))) {
			if (logData) {
				logger.lock();
				std::stringstream ss;
				ss << myGetElapsedTimeMillis() << "," << vLogFormat << "," << KEY_DOWN_CODE <<
					key << ",\n";
				logger.loggerQueue.push(ss.str());
				logger.unlock();
			}
			experimentGovernor.buttonPressed();
		}
		keyTracker.setKeyState(((char) key), true);
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	if ((((char) key) == '0') || (((char) key) == '1') || (((char) key) == '2')
		 || (((char) key) == '3')  || (((char) key) == '4')  || (((char) key) == '5'))	{
		if (!keyTracker.getKeyState(((char) key))) {
			if (logData) {
				logger.lock();
				std::stringstream ss;
				ss << myGetElapsedTimeMillis() << "," << vLogFormat << "," << KEY_DOWN_CODE <<
					key << ",\n";
				logger.loggerQueue.push(ss.str());
				logger.unlock();
			}
			experimentGovernor.buttonPressed();
		}
		keyTracker.setKeyState(((char) key), false);
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