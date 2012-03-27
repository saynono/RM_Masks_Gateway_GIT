/*
 *  MaskLoop.h
 *  moviePlayerExample
 *
 *  Created by say nono on 06.07.11.
 *  Copyright 2011 www.say-nono.com. All rights reserved.
 *
 */

#pragma once


#include "ofMain.h"
#include "ofxNetwork.h"

struct FrameData{
	int id;
	int time;
	int size;
	ofxTCPClient* connectionOut;
	char frame[2000];
};


class MaskLoop : public ofThread{

public:
	MaskLoop();
	~MaskLoop();
	
	void start();
	void stop();
	
	void threadedFunction();
	
	void addFrame(FrameData* frame);
	void sendFrame(vector<FrameData>  *frames);
	
	float		framerate;
	int			lastFrameTime;
	float		sleepTime;
	
	vector<FrameData>  framesMask1;
	vector<FrameData>  framesMask2;
	vector<FrameData>  framesMask3;
	vector<FrameData>  framesMask4;

	
};