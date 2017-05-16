// Comb filter class declaration
//
// Written by Jezar at Dreampoint, June 2000
// http://www.dreampoint.co.uk
// This code is public domain

#ifndef _comb_
#define _comb_

#include "denormals.h"
#include "JuceHeader.h"

//This class was largely written by Jezar but has been modified to make the tuning of the comb filter variable.
class comb
{
public:
					comb();
			void	setbuffer(float *buf, int size);
	inline  float	process(float inp);
			void	mute();
			void	setdamp(float val);
			float	getdamp();
			void	setfeedback(float val);
			float	getfeedback();
    
    //This was my addition to the class:
    void setbufsize(const float& val);
    
private:
	float	feedback;
	float	filterstore;
	float	damp1;
	float	damp2;
	float	*buffer;
	int		bufsize;
	int		bufidx;
};


//Jezar's comment:
// Big to inline - but crucial for speed
inline float comb::process(float input)
{
    //We check this at the begining of the loop incase a change in buffersize made bufidx go out of bounds.
    //Originally bufsize was a constant value that did not change.
    if(bufidx>=bufsize) bufidx = 0;
    
    //Store the current value in our comb filter at this point.
    float output;
	output = buffer[bufidx];
    
    //Do something good to it?
	undenormalise(output);

    //Filter the current value in our comb filter by combining it with the previous value in our comb filter multiplied by some dampening values.
    //Basically smoothing the value with it's last value
	filterstore = (output*damp2) + (filterstore*damp1);
    
    //Do not know what this does (Jezar's functionality)
	undenormalise(filterstore);

    //Replace the current value in our comb filter with the input combined with the current value in our comb filter multiplied by a feedback value.
    buffer[bufidx] = input + (filterstore*feedback);
    
    //Incrememnt the buffer index
    bufidx++;
    
    //Output the pre-modified value in our comb filter.
	return output;
}

#endif //_comb_

//ends
