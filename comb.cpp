// Comb filter implementation
//
// Written by Jezar at Dreampoint, June 2000
// http://www.dreampoint.co.uk
// This code is public domain

#include "comb.hpp"

//All these functions are written by Jezar except set buffer size.
comb::comb()
{
	filterstore = 0;
	bufidx = 0;
}

//Allocating what buffer to edit.
void comb::setbuffer(float *buf, int size)
{
	buffer = buf; 
	bufsize = size;
}

//Making all values in the buffer 0.
void comb::mute()
{
	for (int i=0; i<bufsize; i++)
		buffer[i]=0;
}

void comb::setfeedback(float val) 
{
	feedback = val;
}

float comb::getfeedback() 
{
	return feedback;
}

void comb::setbufsize(const float& val)
{
    bufsize = val;
}





// ends
