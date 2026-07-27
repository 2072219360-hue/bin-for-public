#pragma once

#include<iostream>
class myColorTable
{
public:
	myColorTable();
	~myColorTable();
	unsigned char(*colorTable)[3];
	int colorNum;
	unsigned char myRed[3] = { 255, 0, 0 };
	unsigned char myGreen[3] = { 0, 255, 0 };
	unsigned char myBlue[3] = { 0, 0, 255 };
	unsigned char myYellow[3] = { 255,255,0 };
	unsigned char myBlack[3] = { 0,0,0 };
	unsigned char mySilver[3] = { 192,192,192 };
	unsigned char myPurple[3] = { 128,0,128 };
	unsigned char myBrown[3] = { 165,42,42 };
};

