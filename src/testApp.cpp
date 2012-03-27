#include "testApp.h"

#define MASK_WIDTH		16 //20
#define MASK_HEIGHT		28 //16
#define LED_WIDTH		100 //20
#define LED_HEIGHT		4 //16
#define APP_TITLE		"RHYTHM MONKS Serial Gateway v5";

//--------------------------------------------------------------
void testApp::setup(){
	
//	ofSetDataPathRoot("./data/");
	
	appTitle = APP_TITLE;
	ofSetWindowTitle(appTitle);
	//load our type
	mono.loadFont("type/mono.ttf", 8);
	monosm.loadFont("type/mono.ttf", 6);
	
	doUDP = false;
	
	if(xmlSettings.loadFile("serialsettings.xml")){		
		int numMasks = xmlSettings.getNumTags("settings:masks:mask");
		xmlSettings.pushTag("settings", 0);
		int amount = xmlSettings.getAttribute("masks", "amount", 4, 0);
//		bool udp = xmlSettings.getAttribute("masks", "tcp_in", false, 0);
		doUDP = !xmlSettings.getAttribute("masks", "tcp_out", 0, 0);
		printf("doUDP : %i \n",doUDP);
		xmlSettings.pushTag("masks", 0);
		for(int i=0;i<amount;i++){
			if(xmlSettings.tagExists("mask", i)){
				xmlSettings.pushTag("mask", i);
				maskActive[i]	= xmlSettings.getValue("active", true, 0);
				maskNames[i]	= xmlSettings.getValue("mask_name", "Ben Stiller", 0);
				portsOut[i]		= xmlSettings.getValue("port_sending", (5301+i), 0);
				portsIn[i]		= xmlSettings.getValue("port_listening", (5301+i), 0);
				ipOut[i]		= xmlSettings.getValue("ip_sending", "192.168.1.161", 0);
				descriptions[i] = maskNames[i] + "\n\nPort_in  : " + ofToString(portsIn[i]) + "\n" + "Port_out : " + ofToString(portsOut[i]) + "\n" "IP_out    : " + ipOut[i] + "\n" ;
				xmlSettings.popTag();
			}
		}
	};
	
	setTestData();
	
	ofSetFrameRate(25);
	
    //create the socket and bind to port 11999
	
	udpConnectionIn1.Create();
	udpConnectionIn1.Bind(portsIn[0]);
	udpConnectionIn1.SetNonBlocking(true);

	udpConnectionIn2.Create();
	udpConnectionIn2.Bind(portsIn[1]);
	udpConnectionIn2.SetNonBlocking(true);

	udpConnectionIn3.Create();
	udpConnectionIn3.Bind(portsIn[2]);
	udpConnectionIn3.SetNonBlocking(true);

	udpConnectionIn4.Create();
	udpConnectionIn4.Bind(portsIn[3]);
	udpConnectionIn4.SetNonBlocking(true);
	
//	image1.allocate(MASK_WIDTH, MASK_HEIGHT, OF_IMAGE_COLOR_ALPHA);
	image1.allocate(16, 28, OF_IMAGE_COLOR);
	image2.allocate(16, 28, OF_IMAGE_COLOR);
	image3.allocate(16, 28, OF_IMAGE_COLOR);
	image4.allocate(16, 28, OF_IMAGE_COLOR);
	
	image1.loadImage("images/Maske_16.tif");
	image2.loadImage("images/Maske_16.tif");
	image3.loadImage("images/Maske_16.tif");
	image4.loadImage("images/Maske_16.tif");
		
	
	if(doUDP){	

		udpConnectionOut1.Create();
		udpConnectionOut1.Connect(ipOut[0].c_str(),portsOut[0]);
		udpConnectionOut1.SetNonBlocking(true);
	
		udpConnectionOut2.Create();
		udpConnectionOut2.Connect(ipOut[1].c_str(),portsOut[1]);
		udpConnectionOut2.SetNonBlocking(true);
	
		udpConnectionOut3.Create();
		udpConnectionOut3.Connect(ipOut[2].c_str(),portsOut[2]);
		udpConnectionOut3.SetNonBlocking(true);
	
		udpConnectionOut4.Create();
		udpConnectionOut4.Connect(ipOut[3].c_str(),portsOut[3]);
		udpConnectionOut4.SetNonBlocking(true);
	}else{
		tcpConnected[0] = false;
		tcpConnected[1] = false;
		tcpConnected[2] = false;
		tcpConnected[3] = false;	
	
		tcpConnections[0] = &tcpConnectionOut1;
		tcpConnections[1] = &tcpConnectionOut2;
		tcpConnections[2] = &tcpConnectionOut3;
		tcpConnections[3] = &tcpConnectionOut4;
	
//	tcpConnected[0] = tcpConnectionOut1.setup(ipOut[0].c_str(),portsOut[0]);
//	tcpConnected[1] = tcpConnectionOut2.setup(ipOut[1].c_str(),portsOut[1]);
//	tcpConnected[2] = tcpConnectionOut3.setup(ipOut[2].c_str(),portsOut[2]);
//	tcpConnected[3] = tcpConnectionOut4.setup(ipOut[3].c_str(),portsOut[3]);
	
		tcpConnectionOut4.setVerbose(true);
		tcpConnectionOut3.setVerbose(true);
		tcpConnectionOut2.setVerbose(true);
		tcpConnectionOut1.setVerbose(true);
	}
	
	
//	tcpServer.setVerbose(true);
//	tcpServer.setup(6001);

	float moduleOrderTemp[] = {0,1,2,3,7,6,5,4,9.5,15,14,13,12,16,17,18,19,23,22,21,20,24,25,26,27};
	for (int i=0; i<25; i++) {
		float val = moduleOrderTemp[i];
		moduleOrder[i] = (int) (val/4) * 4 + fmod(val,4);
	}
	
	isTestMode = false;
	
	
//	maskLoop.start();
	
}

//--------------------------------------------------------------
void testApp::update(){
	
	if(doUDP){
		receiveData();
	}else{
		reconnect();
		receiveDataTCP();
//		tcpConnectionOut1.send("hi");
		
	}
	
}

//--------------------------------------------------------------
void testApp::draw(){
	int px = 10;
	int py = 70;
	ofBackground(30, 30, 30);
	ofSetColor(160, 160, 160);
	mono.drawString(appTitle, 15, 30);
	
	ofTranslate(0, py, 0);	
	
	ofSetColor(255,255,255);
	ofTranslate(px, 0, 0);
	drawImage(&image1,0,0,160,280);
	
	ofSetColor(160, 160, 160);
	monosm.drawString(descriptions[0], 5, 300);
	if(tcpConnected[0]) ofSetColor(50,200,50);
	else ofSetColor(200,50,50);
	if(!doUDP) ofCircle(0, 300, 5);
	ofSetColor(255,255,255);
	
	px += 160;
	ofTranslate(px, 0, 0);
	drawImage(&image2,0,0,160,280);
	ofSetColor(160, 160, 160);
	monosm.drawString(descriptions[1], 5, 300);
	if(tcpConnected[1]) ofSetColor(50,200,50);
	else ofSetColor(200,50,50);
	if(!doUDP) ofCircle(0, 300, 5);
	ofSetColor(255,255,255);
	
	ofTranslate(px, 0, 0);
	drawImage(&image3,0,0,160,280);
	ofSetColor(160, 160, 160);
	monosm.drawString(descriptions[2], 5, 300);
	if(tcpConnected[2]) ofSetColor(50,200,50);
	else ofSetColor(200,50,50);
	if(!doUDP) ofCircle(0, 300, 5);
	ofSetColor(255,255,255);
	
	ofTranslate(px, 0, 0);
	drawImage(&image4,0,0,160,280);
	ofSetColor(160, 160, 160);
	monosm.drawString(descriptions[3], 5, 300);
	if(tcpConnected[3]) ofSetColor(50,200,50);
	else ofSetColor(200,50,50);
	if(!doUDP) ofCircle(0, 300, 5);
	ofSetColor(255,255,255);
	
}

void testApp::drawImage(ofImage* img,int x, int y, int w, int h){
	
	int wOrg = img->getWidth();
	int hOrg = img->getHeight();
	
	float wStep = wOrg/(float)w;
	float hStep = hOrg/(float)h;
	wStep = 10;
	hStep = 10;
	ofColor clr;
	int xs,ys;
	for(xs=0;xs<wOrg;xs++){
		for(ys=0;ys<hOrg;ys++){
			clr = img->getColor(xs, ys);
			ofSetHexColor(clr.getHex());
			ofFill();
			ofSetLineWidth(0);
			ofRect(xs*wStep+x, ys*hStep+y, wStep, hStep);
			ofSetLineWidth(1);
			ofNoFill();
			ofEnableAlphaBlending();
			ofSetColor(180, 180, 180,50);
			ofRect(xs*wStep+x, ys*hStep+y, wStep, hStep);
			ofDisableAlphaBlending();
		}
	}
	ofFill();
	
	ofSetColor(	30, 30, 30);
	ofRect(x-1, 8*hStep+y+1, wStep*6, hStep*4-1);
	ofRect(10*wStep+x, 8*hStep+y+1, wStep*6, hStep*4-1);
	
}

void testApp::receiveData(){
	checkForData(&udpConnectionIn1,&udpConnectionOut1,&image1);
	checkForData(&udpConnectionIn2,&udpConnectionOut2,&image2);
	checkForData(&udpConnectionIn3,&udpConnectionOut3,&image3);
	checkForData(&udpConnectionIn4,&udpConnectionOut4,&image4);	
}

void testApp::receiveDataTCP(){
	checkForDataUDPToTCP(0,&udpConnectionIn1,&tcpConnectionOut1,&image1);
	checkForDataUDPToTCP(1,&udpConnectionIn2,&tcpConnectionOut2,&image2);
	checkForDataUDPToTCP(2,&udpConnectionIn3,&tcpConnectionOut3,&image3);
	checkForDataUDPToTCP(3,&udpConnectionIn4,&tcpConnectionOut4,&image4);	
}

void testApp::sendTestData(){
	sendTestDataPort(&udpConnectionOut1,&image1);
	sendTestDataPort(&udpConnectionOut2,&image2);
	sendTestDataPort(&udpConnectionOut3,&image3);
	sendTestDataPort(&udpConnectionOut4,&image4);	
}


void testApp::reconnect(){
	reconnectTCP(0);
	reconnectTCP(1);
	reconnectTCP(2);
	reconnectTCP(3);	
}

void testApp::reconnectTCP(int portID){
	int curFrame = ofGetFrameNum();
	if(!maskActive[portID]) return;
	if(tcpConnections[portID]->isConnected()) return;
//	printf("\n\n\n%i   timecheck : cur: %i    <=> %i    ===> %i\n",portID,curFrame,tcpConnectTimeAttept[portID],curFrame-tcpConnectTimeAttept[portID]);
	if(curFrame-tcpConnectTimeAttept[portID] < 0) return;

	string pingStr = (string)"ping -c 1 -t 1 -n " + ipOut[portID]; 
	int flag = system( pingStr.c_str());   
	if(flag != 0){
		printf("Client seems to be offline( %s ) -> code: %i\n",ipOut[portID].c_str(),flag);
		printf("%i   double check 		tcpConnectTimeAttept[portID]: %i      curFrame: %i\n",portID,tcpConnectTimeAttept[portID],curFrame);
		tcpConnectTimeAttept[portID] = curFrame+10000;
		return;
	}
	
	printf("Reconnecting to %s [%i]     %i    ... ", ipOut[portID].c_str(),portsOut[portID],(tcpConnectTimeAttept[portID] - (curFrame)));
	tcpConnected[portID] = tcpConnections[portID]->setup(ipOut[portID].c_str(),portsOut[portID],false);	
	tcpConnectTimeAttept[portID] = ofGetFrameNum()+500;
	if(tcpConnected[portID]) printf("succeded.\n\n\n");
	else printf("failed!\n\n\n");
}

bool testApp::sendTestDataPort(ofxUDPManager* connectionOut,ofImage* img){
	
	int bytesSend = LED_WIDTH * LED_HEIGHT * 3 + 12;
	string message=udpMessage3;
	if(message!=""){
		//		char address[50];
		//		connection->GetRemoteAddr(address);
		
		int offsetByte = 12;
		
		int posVideo;
		int posMask;
		float slot;
		float row;
		int w = 16;
		int videoWidth = (int)(w/4);
		float posModule;
		
		unsigned char image[3000];
		for(int i=0;i<3000;i++) image[i] = 0;
		
		for (int i=0; i<25; i++) {
			posModule = moduleOrder[i];
			row = (int)(posModule/4);
			slot = fmod(posModule,(float)videoWidth);
			if(posModule >= 0){
				for (int j=0; j<16; j++) {				
					posVideo = (int)(j%4)+(int)((j/4)*w) + slot*4 + row*4*w;
					posMask = (int)j%4+(int)(j/4)*LED_WIDTH + i*4;
					posMask *= 3;
					image[posVideo*3] = udpMessage3[posMask+offsetByte];
					image[posVideo*3+1] = udpMessage3[posMask+1+offsetByte];
					image[posVideo*3+2] = udpMessage3[posMask+2+offsetByte];
					
				}
			}
		}		
		connectionOut->Send(udpMessage3, bytesSend);
		//		image1.setFromPixels((unsigned char*)udpMessage+12, 16, 28, OF_IMAGE_COLOR, GL_RGB);
		img->setFromPixels(image, 16, 28, OF_IMAGE_COLOR, GL_RGB);
		return true;
	}
	return false;	
}



bool testApp::checkForData(ofxUDPManager* connectionIn,ofxUDPManager* connectionOut,ofImage* img){
	
	int bytesSend = LED_WIDTH * LED_HEIGHT * 3 + 12;
	
	char udpMessage[2000];
	int ret = connectionIn->Receive(udpMessage,2000);	
	string message=udpMessage;
//	printf("return : %i\n",ret);
	if(message!=""){
//		char address[50];
//		connection->GetRemoteAddr(address);
		
		int offsetByte = 12;
		
		int posVideo;
		int posMask;
		float slot;
		float row;
		int w = 16;
		int videoWidth = (int)(w/4);
		float posModule;
		
		unsigned char image[3000];
		for(int i=0;i<3000;i++) image[i] = 0;
		
		for (int i=0; i<25; i++) {
			posModule = moduleOrder[i];
			row = (int)(posModule/4);
			slot = fmod(posModule,(float)videoWidth);
			if(posModule >= 0){
				for (int j=0; j<16; j++) {				
					posVideo = (int)(j%4)+(int)((j/4)*w) + slot*4 + row*4*w;
					posMask = (int)j%4+(int)(j/4)*LED_WIDTH + i*4;
					posMask *= 3;
					image[posVideo*3] = udpMessage[posMask+offsetByte];
					image[posVideo*3+1] = udpMessage[posMask+1+offsetByte];
					image[posVideo*3+2] = udpMessage[posMask+2+offsetByte];
					
				}
			}
		}		
		
		connectionOut->Send(udpMessage, bytesSend);
		//		image1.setFromPixels((unsigned char*)udpMessage+12, 16, 28, OF_IMAGE_COLOR, GL_RGB);
		img->setFromPixels(image, 16, 28, OF_IMAGE_COLOR, GL_RGB);
		return true;
	}
	return false;
}


bool testApp::checkForDataUDPToTCP(int id,ofxUDPManager* connectionIn,ofxTCPClient* connectionOut,ofImage* img){
	
	int bytesSend = LED_WIDTH * LED_HEIGHT * 3 + 12;
	if(!maskActive[id]) return false;
	FrameData fd;
	fd.size = bytesSend;
	fd.connectionOut = connectionOut;
	fd.time = ofGetElapsedTimeMillis();
	fd.id = id;
//	char udpMessage[2000];
	int ret = connectionIn->Receive(fd.frame,2000);	
	string message=fd.frame;
//		printf("bytesSend : %i\n",bytesSend);
	if(message!=""){
		//		char address[50];
		//		connection->GetRemoteAddr(address);
		
		int offsetByte = 12;
		
		int posVideo;
		int posMask;
		float slot;
		float row;
		int w = 16;
		int videoWidth = (int)(w/4);
		float posModule;
		
		unsigned char image[3000];
		for(int i=0;i<3000;i++) image[i] = 0;
		
		for (int i=0; i<25; i++) {
			posModule = moduleOrder[i];
			row = (int)(posModule/4);
			slot = fmod(posModule,(float)videoWidth);
			if(posModule >= 0){
				for (int j=0; j<16; j++) {				
					posVideo = (int)(j%4)+(int)((j/4)*w) + slot*4 + row*4*w;
					posMask = (int)j%4+(int)(j/4)*LED_WIDTH + i*4;
					posMask *= 3;
					image[posVideo*3] = fd.frame[posMask+offsetByte];
					image[posVideo*3+1] = fd.frame[posMask+1+offsetByte];
					image[posVideo*3+2] = fd.frame[posMask+2+offsetByte];
					
				}
			}
		}		
		
		maskLoop.addFrame(&fd);
		
		
//		connectionOut->sendRawBytes(fd.frame, bytesSend);
		//		image1.setFromPixels((unsigned char*)udpMessage+12, 16, 28, OF_IMAGE_COLOR, GL_RGB);
		img->setFromPixels(image, 16, 28, OF_IMAGE_COLOR, GL_RGB);
		return true;
	}
	return false;
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	if(key == ' '){
//		isTestMode = !isTestMode;
//		if(isTestMode) 	ofSetFrameRate(20);
//		else ofSetFrameRate(120);
//		image1.allocate(16, 28, OF_IMAGE_COLOR);
//		image2.allocate(16, 28, OF_IMAGE_COLOR);
//		image3.allocate(16, 28, OF_IMAGE_COLOR);
//		image4.allocate(16, 28, OF_IMAGE_COLOR);	
	}
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

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


void testApp::setTestData(){
	char udpMessage[] = { 0xff, 0x0B, 0x00, 0x03, 0x14, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x84, 0xFE, 0x00, 0xBB, 0xFE, 0x73,
	0x83, 0xFE, 0x00, 0xF9, 0xFE, 0xF2, 0x84, 0xFE, 0x00, 0xAF,
	0xFE, 0x59, 0x83, 0xFE, 0x00, 0x83, 0xFE, 0x00, 0x84, 0xFE,
	0x00, 0x84, 0xFE, 0x00, 0x96, 0xFE, 0x26, 0x84, 0xFE, 0x00,
	0xE0, 0xFE, 0xBF, 0x84, 0xFE, 0x00, 0xD4, 0xFE, 0xA6, 0x84,
	0xFE, 0x00, 0x8A, 0xFE, 0x0D, 0x84, 0xFE, 0x00, 0x84, 0xFE,
	0x00, 0x84, 0xFE, 0x00, 0x98, 0xFE, 0x00, 0xC7, 0xFE, 0x73,
	0x98, 0xFE, 0x00, 0xFA, 0xFE, 0xF2, 0x97, 0xFE, 0x00, 0xBC,
	0xFE, 0x59, 0x97, 0xFE, 0x00, 0x98, 0xFE, 0x00, 0x98, 0xFE,
	0x00, 0x98, 0xFE, 0x00, 0xA7, 0xFE, 0x26, 0x98, 0xFE, 0x00,
	0xE5, 0xFE, 0xBF, 0x97, 0xFE, 0x00, 0xDB, 0xFE, 0xA6, 0x97,
	0xFE, 0x00, 0x9D, 0xFE, 0x0D, 0x98, 0xFE, 0x00, 0x98, 0xFE,
	0x00, 0x97, 0xFE, 0x00, 0xAB, 0xFE, 0x00, 0xD1, 0xFE, 0x73,
	0xAB, 0xFE, 0x00, 0xFB, 0xFE, 0xF2, 0xAB, 0xFE, 0x00, 0xC9,
	0xFE, 0x59, 0xAC, 0xFE, 0x00, 0xAC, 0xFE, 0x00, 0xAC, 0xFE,
	0x00, 0xAB, 0xFE, 0x00, 0xB9, 0xFE, 0x26, 0xAB, 0xFE, 0x00,
	0xEA, 0xFE, 0xBF, 0xAB, 0xFE, 0x00, 0xE2, 0xFE, 0xA6, 0xAC,
	0xFE, 0x00, 0xB0, 0xFE, 0x0D, 0xAC, 0xFE, 0x00, 0xAC, 0xFE,
	0x00, 0xAB, 0xFE, 0x00, 0xBF, 0xFE, 0x00, 0xDB, 0xFE, 0x73,
	0xBF, 0xFE, 0x00, 0xFC, 0xFE, 0xF2, 0xBF, 0xFE, 0x00, 0xD5,
	0xFE, 0x59, 0xBF, 0xFE, 0x00, 0xBF, 0xFE, 0x00, 0xBF, 0xFE,
	0x00, 0xBF, 0xFE, 0x00, 0xC8, 0xFE, 0x26, 0xBF, 0xFE, 0x00,
	0xEF, 0xFE, 0xBF, 0xBF, 0xFE, 0x00, 0xE8, 0xFE, 0xA6, 0xBF,
	0xFE, 0x00, 0xC1, 0xFE, 0x0D, 0xBE, 0xFE, 0x00, 0xBE, 0xFE,
	0x00, 0xBE, 0xFE, 0x00, 0xD0, 0xFE, 0x00, 0xE5, 0xFE, 0x73,
	0xD0, 0xFE, 0x00, 0xFD, 0xFE, 0xF2, 0xD0, 0xFE, 0x00, 0xE1,
	0xFE, 0x59, 0xCF, 0xFE, 0x00, 0xD0, 0xFE, 0x00, 0xD0, 0xFE,
	0x00, 0xCF, 0xFE, 0x00, 0xD7, 0xFE, 0x26, 0xD0, 0xFE, 0x00,
	0xF3, 0xFE, 0xBF, 0xD0, 0xFE, 0x00, 0xEF, 0xFE, 0xA6, 0xD0,
	0xFE, 0x00, 0xD3, 0xFE, 0x0D, 0xD0, 0xFE, 0x00, 0xD0, 0xFE,
	0x00, 0xD0, 0xFE, 0x00, 0xE0, 0xFE, 0x00, 0xEE, 0xFE, 0x73,
	0xE0, 0xFE, 0x00, 0xFD, 0xFE, 0xF2, 0xE0, 0xFE, 0x00, 0xEB,
	0xFE, 0x59, 0xE0, 0xFE, 0x00, 0xE1, 0xFE, 0x00, 0xE1, 0xFE,
	0x00, 0xE0, 0xFE, 0x00, 0xE4, 0xFE, 0x26, 0xE0, 0xFE, 0x00,
	0xF7, 0xFE, 0xBF, 0xE0, 0xFE, 0x00, 0xF4, 0xFE, 0xA6, 0xE0,
	0xFE, 0x00, 0xE1, 0xFE, 0x0D, 0xE0, 0xFE, 0x00, 0xE1, 0xFE,
	0x00, 0xE0, 0xFE, 0x00, 0xEE, 0xFE, 0x00, 0xF6, 0xFE, 0x73,
	0xEE, 0xFE, 0x00, 0xFE, 0xFE, 0xF2, 0xEE, 0xFE, 0x00, 0xF4,
	0xFE, 0x59, 0xEE, 0xFE, 0x00, 0xEE, 0xFE, 0x00, 0xEE, 0xFE,
	0x00, 0xEE, 0xFE, 0x00, 0xF1, 0xFE, 0x26, 0xEE, 0xFE, 0x00,
	0xFA, 0xFE, 0xBF, 0xEE, 0xFE, 0x00, 0xF9, 0xFE, 0xA6, 0xEE,
	0xFE, 0x00, 0xEE, 0xFE, 0x0D, 0xEE, 0xFE, 0x00, 0xEE, 0xFE,
	0x00, 0xEE, 0xFE, 0x00, 0xF9, 0xFE, 0x00, 0xFC, 0xFE, 0x73,
	0xF9, 0xFE, 0x00, 0xFE, 0xFE, 0xF2, 0xF9, 0xFE, 0x00, 0xFB,
	0xFE, 0x59, 0xF9, 0xFE, 0x00, 0xF9, 0xFE, 0x00, 0xF9, 0xFE,
	0x00, 0xF9, 0xFE, 0x00, 0xFA, 0xFE, 0x26, 0xF9, 0xFE, 0x00,
	0xFE, 0xFE, 0xBF, 0xF9, 0xFE, 0x00, 0xFD, 0xFE, 0xA6, 0xF9,
	0xFE, 0x00, 0xF9, 0xFE, 0x0D, 0xF9, 0xFE, 0x00, 0xF9, 0xFE,
	0x00, 0xF9, 0xFE, 0x00, 0xFE, 0xFC, 0x00, 0xFE, 0xFD, 0x73,
	0xFE, 0xFC, 0x00, 0xFE, 0xFE, 0xF2, 0xFE, 0xFC, 0x00, 0xFE,
	0xFD, 0x59, 0xFE, 0xFB, 0x00, 0xFE, 0xFC, 0x00, 0xFE, 0xFC,
	0x00, 0xFE, 0xFB, 0x00, 0xFE, 0xFC, 0x26, 0xFE, 0xFB, 0x00,
	0xFE, 0xFE, 0xBF, 0xFE, 0xFC, 0x00, 0xFE, 0xFE, 0xA6, 0xFE,
	0xFC, 0x00, 0xFE, 0xFC, 0x0D, 0xFE, 0xFC, 0x00, 0xFE, 0xFC,
	0x00, 0xFE, 0xFB, 0x00, 0xFE, 0xF0, 0x00, 0xFE, 0xF7, 0x73,
	0xFE, 0xF0, 0x00, 0xFE, 0xFE, 0xF2, 0xFE, 0xF0, 0x00, 0xFE,
	0xF6, 0x59, 0xFE, 0xF1, 0x00, 0xFE, 0xF0, 0x00, 0xFE, 0xF0,
	0x00, 0xFE, 0xF0, 0x00, 0xFE, 0xF2, 0x26, 0xFE, 0xF0, 0x00,
	0xFE, 0xFB, 0xBF, 0xFE, 0xF0, 0x00, 0xFE, 0xFA, 0xA6, 0xFE,
	0xF0, 0x00, 0xFE, 0xF1, 0x0D, 0xFE, 0xF0, 0x00, 0xFE, 0xF0,
	0x00, 0xFE, 0xF1, 0x00, 0xFE, 0xE2, 0x00, 0xFE, 0xEF, 0x73,
	0xFE, 0xE2, 0x00, 0xFE, 0xFE, 0xF2, 0xFE, 0xE2, 0x00, 0xFE,
	0xEC, 0x59, 0xFE, 0xE2, 0x00, 0xFE, 0xE2, 0x00, 0xFE, 0xE2,
	0x00, 0xFE, 0xE3, 0x00, 0xFE, 0xE6, 0x26, 0xFE, 0xE2, 0x00,
	0xFE, 0xF8, 0xBF, 0xFE, 0xE3, 0x00, 0xFE, 0xF5, 0xA6, 0xFE,
	0xE2, 0x00, 0xFE, 0xE3, 0x0D, 0xFE, 0xE2, 0x00, 0xFE, 0xE2,
	0x00, 0xFE, 0xE2, 0x00, 0xFE, 0xD1, 0x00, 0xFE, 0xE6, 0x73,
	0xFE, 0xD1, 0x00, 0xFE, 0xFD, 0xF2, 0xFE, 0xD1, 0x00, 0xFE,
	0xE1, 0x59, 0xFE, 0xD0, 0x00, 0xFE, 0xD1, 0x00, 0xFE, 0xD1,
	0x00, 0xFE, 0xD1, 0x00, 0xFE, 0xD8, 0x26, 0xFE, 0xD1, 0x00,
	0xFE, 0xF4, 0xBF, 0xFE, 0xD0, 0x00, 0xFE, 0xEF, 0xA6, 0xFE,
	0xD1, 0x00, 0xFE, 0xD4, 0x0D, 0xFE, 0xD1, 0x00, 0xFE, 0xD1,
	0x00, 0xFE, 0xD1, 0x00, 0xFE, 0xBF, 0x00, 0xFE, 0xDC, 0x73,
	0xFE, 0xBF, 0x00, 0xFE, 0xFC, 0xF2, 0xFE, 0xBF, 0x00, 0xFE,
	0xD5, 0x59, 0xFE, 0xBF, 0x00, 0xFE, 0xBF, 0x00, 0xFE, 0xBF,
	0x00, 0xFE, 0xBF, 0x00, 0xFE, 0xC8, 0x26, 0xFE, 0xBF, 0x00,
	0xFE, 0xEF, 0xBF, 0xFE, 0xBF, 0x00, 0xFE, 0xE9, 0xA6, 0xFE,
	0xBE, 0x00, 0xFE, 0xC2, 0x0D, 0xFE, 0xBF, 0x00, 0xFE, 0xBF,
	0x00, 0xFE, 0xBF, 0x00, 0xFE, 0xAA, 0x00, 0xFE, 0xD1, 0x73,
	0xFE, 0xAA, 0x00, 0xFE, 0xFB, 0xF2, 0xFE, 0xAB, 0x00, 0xFE,
	0xC8, 0x59, 0xFE, 0xAB, 0x00, 0xFE, 0xAB, 0x00, 0xFE, 0xAA,
	0x00, 0xFE, 0xAB, 0x00, 0xFE, 0xB7, 0x26, 0xFE, 0xAA, 0x00,
	0xFE, 0xEA, 0xBF, 0xFE, 0xAA, 0x00, 0xFE, 0xE2, 0xA6, 0xFE,
	0xAA, 0x00, 0xFE, 0xAF, 0x0D, 0xFE, 0xAA, 0x00, 0xFE, 0xAA,
	0x00, 0xFE, 0xAA, 0x00, 0xFE, 0x96, 0x00, 0xFE, 0xC5, 0x73,
	0xFE, 0x95, 0x00, 0xFE, 0xFA, 0xF2, 0xFE, 0x95, 0x00, 0xFE,
	0xBB, 0x59, 0xFE, 0x95, 0x00, 0xFE, 0x95, 0x00, 0xFE, 0x95,
	0x00, 0xFE, 0x95, 0x00, 0xFE, 0xA6, 0x26, 0xFE, 0x96, 0x00,
	0xFE, 0xE5, 0xBF, 0xFE, 0x96, 0x00, 0xFE, 0xDA, 0xA6, 0xFE,
	0x95, 0x00, 0xFE, 0x9B, 0x0D, 0xFE, 0x95, 0x00, 0xFE, 0x95,
	0x00, 0xFE, 0x95, 0x00, 0xFE, 0x80, 0x00, 0xFE, 0xB9, 0x73,
	0xFE, 0x80, 0x00, 0xFE, 0xF9, 0xF2, 0xFE, 0x80, 0x00, 0xFE,
	0xAD, 0x59, 0xFE, 0x80, 0x00, 0xFE, 0x80, 0x00, 0xFE, 0x80,
	0x00, 0xFE, 0x80, 0x00, 0xFE, 0x93, 0x26, 0xFE, 0x80, 0x00,
	0xFE, 0xDF, 0xBF, 0xFE, 0x80, 0x00, 0xFE, 0xD3, 0xA6, 0xFE,
	0x80, 0x00, 0xFE, 0x86, 0x0D, 0xFE, 0x80, 0x00, 0xFE, 0x80,
	0x00, 0xFE, 0x80, 0x00 };
	for(int i=0;i<sizeof(udpMessage);i++){
		udpMessage3[i] = udpMessage[i];
	}
//	printf("---: %i  \n",sizeof(udpMessage));

}
