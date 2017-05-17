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
    std::fill(buffer.begin(), buffer.end(), 0.0);
    bufsize = buffer.size();
    inc = 1;
    output = 0.0;
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

void comb::setfrequency(float val)
{
    frequency = val;
//    std::cout << "freq: " << frequency << std::endl;
    inc = bufsize / (44100.0f/frequency);
//    std::cout << "inc: " << bufsize  << std::endl;
}

float comb::getfrequency()
{
    return frequency;
}






// ends
