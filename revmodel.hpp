// Reverb model declaration
//
// Written by Jezar at Dreampoint, June 2000
// http://www.dreampoint.co.uk
// This code is public domain

#ifndef _revmodel_
#define _revmodel_

#include "comb.hpp"
#include "tuning.h"
#include "JuceHeader.h"
#include <array>


//-------------------------------------------------------------
//Comb Filter Class
//-------------------------------------------------------------
//This class is a combination of two comb filters to get a stereo effect
class CombComponent
:
public Component,
Slider::Listener
{
public:
    Slider tuningSlider;
    Label sliderLabel;
    comb combL;
    
    CombComponent();
    CombComponent(const CombComponent& copy);
    
    void sliderValueChanged(Slider* slider) override {};
    void resized() override;
    void	mute();
    void	setfeedback(float val);
    float    processLeft(float val);
    
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
			void	setfeedback(float value);
			float	getfeedback();
			void	setwet(float value);
			float	getwet();
			void	setdry(float value);
			float	getdry();
			void	setmode(float value);
			float	getmode();
    
    ~revmodel();
    
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
        if(button == &originalParamsButton)
            setOriginalParameters();
    }
    void setOriginalParameters();
    
private:
			void	update();
private:
    //Jezar's variables:
	float	gain;
	float	feedback,feedback1;
	float	wet,wet1;
	float	dry;
	float	mode;
    int     index;
    
    static const int combtuning[8];
    int     numcombs    = 8;

    std::vector<CombComponent*> combs;

    //guis
    GroupComponent group;
    TextButton originalParamsButton;
    
};

#endif//_revmodel_

//ends
