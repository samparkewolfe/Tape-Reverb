// Comb filter class declaration
//
// Written by Jezar at Dreampoint, June 2000
// http://www.dreampoint.co.uk
// This code is public domain

#ifndef _comb_
#define _comb_

#include "JuceHeader.h"
#include <array>

//This class was largely written by Jezar but has been modified to make the tuning of the comb filter variable.
class comb
{
public:
					comb();
	inline  float	process(float inp);
			void	mute();
			void	setfeedback(float val);
			float	getfeedback();
            void    setfrequency(float val);
            float   getfrequency();
    
    //This was my addition to the class:
    void setbufsize(const float& val);
    
    std::array<float, 22050>buffer;
    
private:
	float	feedback;
	float	filterstore;
	float	bufsize;
    float   frequency;
    float   inc;
	float	bufidx;
    float   output, j;
};


//Jezar's comment:
// Big to inline - but crucial for speed
inline float comb::process(float input)
{

    //Store the current value in our comb filter at this point.
    for(j = 0; j < inc; j++)
    {
        output = buffer[int(std::fmod(bufidx+j,bufsize))];
        buffer[int(std::fmod(bufidx+j,bufsize))] = input + (output*feedback);
    }
    
    bufidx+=inc;
    bufidx = std::fmod(bufidx, bufsize);
    
    //Output the pre-modified value in our comb filter.
	return output;
}

#endif //_comb_

//ends
