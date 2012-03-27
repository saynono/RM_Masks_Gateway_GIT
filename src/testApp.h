#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxNetwork.h"
#include "ofxXmlSettings.h"
#include "MaskLoop.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
	
		void drawImage(ofImage* img,int x, int y, int w, int h);

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
		void receiveData();
		void receiveDataTCP();
		bool checkForData(ofxUDPManager* connectionIn,ofxUDPManager* connectionOut,ofImage* img);
		bool checkForDataUDPToTCP(int id,ofxUDPManager* connectionIn,ofxTCPClient* connectionOut,ofImage* img);
		void sendTestData();
		bool sendTestDataPort(ofxUDPManager* connectionOut,ofImage* img);
		void setTestData();
	
		void reconnect();
		void reconnectTCP(int portID);
	
	
		ofTrueTypeFont  mono;
		ofTrueTypeFont  monosm;
	
		ofxUDPManager	udpConnectionIn1;
		ofxUDPManager	udpConnectionIn2;
		ofxUDPManager	udpConnectionIn3;
		ofxUDPManager	udpConnectionIn4;
	
		ofxUDPManager	udpConnectionOut1;
		ofxUDPManager	udpConnectionOut2;
		ofxUDPManager	udpConnectionOut3;
		ofxUDPManager	udpConnectionOut4;
	
		ofxTCPClient	tcpConnectionOut1;
		ofxTCPClient	tcpConnectionOut2;
		ofxTCPClient	tcpConnectionOut3;
		ofxTCPClient	tcpConnectionOut4;
		ofxTCPClient*	tcpConnections[4];
	
		bool connectedTo1;
		bool connectedTo2;	
		bool connectedTo3;
		bool connectedTo4;
	
		bool	tcpConnected[4];
		int		tcpConnectTimeAttept[4];
	
		ofxTCPServer tcpServer;

		ofxXmlSettings	xmlSettings;
	
		ofImage			image1;
		ofImage			image2;
		ofImage			image3;
		ofImage			image4;
		
	
//		string			IPout;
		char			udpMessage3[972];
		bool			maskActive[4];
		string			maskNames[4];
		string			ipOut[4];
		string			descriptions[4];
		int				portsOut[4];
		int				portsIn[4];
		float			moduleOrder[25];
	
		bool			isTestMode;
		bool			doUDP;
	
		string			appTitle;

	MaskLoop		maskLoop;
	
	
};

#endif
