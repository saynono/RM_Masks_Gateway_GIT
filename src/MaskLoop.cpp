/*
 *  MaskLoop.cpp
 *  moviePlayerExample
 *
 *  Created by say nono on 06.07.11.
 *  Copyright 2011 www.say-nono.com. All rights reserved.
 *
 */

#include "MaskLoop.h"


MaskLoop::MaskLoop(){
	framerate = 15;//.5f;
	sleepTime = 1000.0f / framerate;
}

MaskLoop::~MaskLoop(){
	stop();
}

void MaskLoop::start(){	
	if(isThreadRunning()) return;
	printf("MaskLoop::start thread()\n");
	startThread(true, false);   // blocking, verbose
}

void MaskLoop::stop(){
	stopThread();
	printf("MaskLoop::stop thread()\n");
}


void MaskLoop::threadedFunction(){
	
	while( isThreadRunning() != 0 ){	
		
		int diff = ofGetElapsedTimeMillis() - lastFrameTime;
		lastFrameTime = ofGetElapsedTimeMillis();
		
		sendFrame(&framesMask1);
		sendFrame(&framesMask2);
		sendFrame(&framesMask3);
		sendFrame(&framesMask4);
		
		ofSleepMillis((int)sleepTime);
	}
	
}


void MaskLoop::addFrame(FrameData* frame){
	if(true){
		lock();
//		printf("frame %i\n",frame->id);
		if(frame->id == 0) framesMask1.push_back(*frame);
		if(frame->id == 1) framesMask2.push_back(*frame);
		if(frame->id == 2) framesMask3.push_back(*frame);
		if(frame->id == 3) framesMask4.push_back(*frame);
		unlock();
	}else{
		frame->connectionOut->sendRawBytes(frame->frame, frame->size);
	}
}

void MaskLoop::sendFrame(vector<FrameData>  *frames){
	if(frames->size() > 0){
		FrameData frame = frames->back();
//		printf("3%i -> frames flush , frames flushed %i\n",frame.id, (int)(frames->size()-1));
		frame.connectionOut->sendRawBytes(frame.frame, frame.size);
		//			frame.connectionOut->send(frame.frame);
		frames->clear();
	}	
}