// Reverb model implementation
//
// Written by Jezar at Dreampoint, June 2000
// http://www.dreampoint.co.uk
// This code is public domain

#include "revmodel.hpp"

//These are the values given by Jezar which are supposed to be static for the unit, this are the values that the reset button returns too.

//Jezzar's comment:
// These values assume 44.1KHz sample rate
// they will probably be OK for 48KHz sample rate
// but would need scaling for 96KHz (or other) sample rates.
// The values were obtained by listening tests.
int const revmodel::combtuning[] =
{
    1116,
    1188,
    1277,
    1356,
    1422,
    1491,
    1557,
    1617,
};


//-------------------------------------------------------------
//Comb Filter Class
//-------------------------------------------------------------
//Stereo Comb Class
CombComponent::CombComponent()
{
    //Setting up gui.
    addAndMakeVisible(&tuningSlider);
    tuningSlider.addListener(this);
    tuningSlider.setSliderStyle(Slider::LinearBarVertical);
    tuningSlider.setRange(0.0, 1.0);
    tuningSlider.setValue(0.5);
    
    addAndMakeVisible(&sliderLabel);
    sliderLabel.setText("Combfilter", dontSendNotification);
    sliderLabel.attachToComponent(&tuningSlider, false);
    
    tuningSlider.setRange(0, 440.0);
}

//Copy constructor.
CombComponent::CombComponent(const CombComponent& copy)
{
    combL = copy.combL;
//    bufcombL = copy.bufcombL;
}

//Basic interfacing to Jezar's comb object.
void CombComponent::mute()
{
    combL.mute();
}

void CombComponent::setfeedback(float val)
{
    combL.setfeedback(val);
}

float CombComponent::processLeft(float val)
{
    combL.setfrequency(tuningSlider.getValue());
    return combL.process(val);
}
 
//Drawing the gui.
void CombComponent::resized()
{
    Rectangle<int> area(getLocalBounds());
    area.removeFromTop(20);
    area.reduce(10, 10);
    tuningSlider.setBounds(area);
}


//-------------------------------------------------------------
//Reverb Model Class
//-------------------------------------------------------------
revmodel::revmodel()
{
    //Guis:
    addAndMakeVisible(&group);
    group.setText("Reverb Back End");
    group.setColour(GroupComponent::outlineColourId, Colours::grey);
    group.toFront(false);
    
    // Initialising the combs
    for(int i = 0; i<numcombs; i++)
    {
        combs.push_back(new CombComponent());
        addAndMakeVisible(combs[i]);
    }
    
    //Initialise the unit to the original settings of Jezars
    setOriginalParameters();
    
    addAndMakeVisible(&originalParamsButton);
    originalParamsButton.setButtonText("Reset");
    originalParamsButton.addListener(this);
    
    
	setwet(initialwet);
	setfeedback(initialroom);
	setdry(initialdry);
	setmode(initialmode);

	// Buffer will be full of rubbish - so we MUST mute them
	mute();    
}

revmodel::~revmodel()
{
    for(auto combComponenet : combs)
        delete combComponenet;
}

//Reseting the untis settings to the original ones.
void revmodel::setOriginalParameters()
{
    for(int i = 0; i<numcombs; i++)
    {
        combs[i]->tuningSlider.setValue((44100.0f/float(combtuning[i])));
    }
}

//Jezar's function:
void revmodel::mute()
{
	if (getmode() >= freezemode)
		return;

	for (int i=0;i<numcombs;i++)
	{
		combs[i]->mute();
	}
}

//This loop is the loop that properly processes the current audio buffer.
void revmodel::processreplace(float *inputL, float *inputR, float *outputL, float *outputR, long numsamples, int skip)
{
	float outL, input;

    //For all the samples in the buffer:
	while(numsamples-- > 0)
	{
		outL = 0;
        input = *inputL * gain;
        
		//From begining to end process the input through all the comb filters
		for(int i=0; i<numcombs; i++)
		{
            outL += combs[i]->processLeft(input);
		}
		*outputL = outL*wet1 + *inputL*dry;
        *outputR = *outputL;
        
		// Increment sample pointers, allowing for interleave (if any)
		inputL += skip;
		inputR += skip;
		outputL += skip;
		outputR += skip;
        index++;
	}
}

//Jezar's function updating the parameters of the unit after a gui change.
//This unit is set up so all guis output between 0-1 and this function scales each 0-1 value indipendantly for the parameter.
void revmodel::update()
{
// Recalculate internal values after parameter change
	int i;
    
    wet1 = wet;

	if (mode >= freezemode)
	{
		feedback1 = 1;
		gain = muted;
	}
	else
	{
		feedback1 = feedback;
		gain = fixedgain;
	}

	for(i=0; i<numcombs; i++)
	{
		combs[i]->setfeedback(feedback1);
	}
}

// Jezar's comment:
// The following get/set functions are not inlined, because
// speed is never an issue when calling them, and also
// because as you develop the reverb model, you may
// wish to take dynamic action when they are called.
void revmodel::setfeedback(float value)
{
    feedback = value; //(value*scaleroom) + offsetroom;
	update();
}
float revmodel::getfeedback()
{
    return feedback; //(feedback-offsetroom)/scaleroom;
}
void revmodel::setwet(float value)
{
	wet = value*scalewet;
	update();
}
float revmodel::getwet()
{
	return wet/scalewet;
}
void revmodel::setdry(float value)
{
	dry = value*scaledry;
}
float revmodel::getdry()
{
	return dry/scaledry;
}

void revmodel::setmode(float value)
{
	mode = value;
	update();
}
float revmodel::getmode()
{
	if (mode >= freezemode)
		return 1;
	else
		return 0;
}

//Drawing the entire back end of the reverb.
void revmodel::resized()
{
    //reset button
    
    Rectangle<int> area(getLocalBounds());
    group.setBounds(area);
    area.reduce(10, 10);
    
    Rectangle<int> areaextra(area.removeFromRight(area.getWidth()*0.1));
    areaextra.reduce(10, 10);
    areaextra.removeFromTop(10);
    originalParamsButton.setBounds(areaextra.removeFromTop(areaextra.getHeight()*0.1));
    areaextra.removeFromTop(20);
    areaextra.removeFromTop(30);
    
    Rectangle<int> areaCombs(area);
    //areaCombs.removeFromBottom(area.getHeight()/2);
    int sliderWidth = (areaCombs.getWidth()/numcombs) ;
    for(int i = 0; i<numcombs; i++)
    {
        combs[i]->setBounds(areaCombs.removeFromLeft(sliderWidth));
    }
}

//ends
