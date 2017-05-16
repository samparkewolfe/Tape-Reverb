// Reverb model declaration
//
// Written by Jezar at Dreampoint, June 2000
// http://www.dreampoint.co.uk
// This code is public domain

#ifndef _revmodel_
#define _revmodel_

#include "comb.hpp"
#include "allpass.hpp"
#include "tuning.h"
#include "JuceHeader.h"
#include <array>


//-------------------------------------------------------------
//Comb Filter Class
//-------------------------------------------------------------
//This class is a combination of two comb filters to get a stereo effect
class stereoComb
:
public Component,
Slider::Listener
{
    
public:
    //This controls the size of the comb filters buffer.
    Slider tuningSlider;
    Label sliderLabel;
    //Left and right comb filters.
    comb combL, combR;
    //These are the buffers that the comb filters write too.
    std::vector<float> bufcombL, bufcombR;
    //The amount of stereo spread this will be variable unlike the "stereospread" variable which is constant and used to initialise bufcombR.
    int thisstereoSpread;
    
    stereoComb();
    stereoComb(const stereoComb& copy);
    
    void sliderValueChanged(Slider* slider) override {};
    void resized() override;
    
    //This function takes a buffersize and fills the bufcombL vectors with the size number of 0's.
    void	setbuffers(int size);
    //This is the built in freeverb function.
    void	mute();
    //Also a freeverb function that will set the filter cutoff value for this comb filter.
    void	setdamp(float val);
    //This will set the feedback value for the comb filter (set by the room size variable)
    void	setfeedback(float val);
    //Each comb filter is processed as L and R to bring in spacialisation. The right buffer is bigger than the left buffer by the stereo spread variable.
    float    processLeft(float val);
    float    processRight(float val);
    //This controlls howmuch larger in samples the right buffer is than the left.
    void    setStereoSpread(const int& val);
    
};


//-------------------------------------------------------------
//Allpass Filter Class
//-------------------------------------------------------------
class stereoAllpass
:
public Component,
Slider::Listener
{
    
public:
    //This controls the size of the comb filters buffer.
    Slider tuningSlider;
    Label sliderLabel;
    //Left and right all pass filters.
    allpass	allpassL, allpassR;
    //These are the buffers that the allpass filters will write too.
        std::vector<float> bufallpassL, bufallpassR;
    //This variable is the same as the comb filters variable.
    int thisstereoSpread;
    
    stereoAllpass();
    stereoAllpass(const stereoAllpass& copy);
    
    void sliderValueChanged(Slider* slider) override {};
    void resized() override;
    
    //This variable is the same as the comb filters variable.
    void	setbuffers(int size);
    //This variable is the same as the comb filters variable.
    void	mute();
    //This variable is the same as the comb filters variable.
    void	setfeedback(float val);
    //This variable is the same as the comb filters variable.
    float    processLeft(float val);
    float    processRight(float val);
    //This variable is the same as the comb filters variable.
    void    setStereoSpread(const int& val);
    
};



//-------------------------------------------------------------
//Reverb Model Class
//-------------------------------------------------------------
class revmodel
:
public Component,
public Slider::Listener,
Button::Listener
{
public:
    
    //All these functions were written by Jezar.
					revmodel();
			void	mute();
			void	processmix(float *inputL, float *inputR, float *outputL, float *outputR, long numsamples, int skip);
			void	processreplace(float *inputL, float *inputR, float *outputL, float *outputR, long numsamples, int skip);
			void	setroomsize(float value);
			float	getroomsize();
			void	setdamp(float value);
			float	getdamp();
			void	setwet(float value);
			float	getwet();
			void	setdry(float value);
			float	getdry();
			void	setwidth(float value);
			float	getwidth();
			void	setmode(float value);
			float	getmode();
    
    //These are my added functionality to the class:
    //Drawing
    void resized() override;
    //Gui threads.
    void sliderValueChanged(Slider* slider) override
    {
        update();
    }
    void buttonClicked(Button* button) override
    {
        //This is triggering the original filter parameters that are declared at the top of the .cpp file.
        if(button == &originalParamsButton)
            setOriginalParameters();
    }
    void setOriginalParameters();
    
private:
			void	update();
private:
    //Jezar's variables:
	float	gain;
	float	roomsize,roomsize1;
	float	damp,damp1;
	float	wet,wet1,wet2;
	float	dry;
	float	width;
	float	mode;
    int     index;
    
    //My Variable:
    //These needed to be static as they were used to set the size of an array however I decided to make the comb filter buffers much larger and in vectors so these being static is a little arbitrary.
    //These store the original filter tunings of jezar's unit.
    static const int combtuning[8];
    static const int allpasstuning[4];
    
    //I made this a separate variable to possibly one day make the number of filters in the program dynamic however one can get the same effect by making the comb filters tunings 0 so I didn't add this.
    int     numcombs    = 8;
    int     numallpasses  = 4;
    
    //Again these were stored in vectors to possibly make dynamic as said above.
    //With the currect code I would put these in arrays.
    // Comb filters
    std::vector<stereoComb*> combs;
    // Allpass filters
    std::vector<stereoAllpass*> allpasses;
    
    //guis
    GroupComponent group;
    Slider stereoSpreadSlider;
    Label stereoSpreadSliderLabel;
    TextButton originalParamsButton;
    
};

#endif//_revmodel_

//ends
