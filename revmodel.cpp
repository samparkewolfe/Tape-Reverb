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
int const revmodel::allpasstuning[] =
{
    556,
    441,
    341,
    225,
};



//-------------------------------------------------------------
//Comb Filter Class
//-------------------------------------------------------------
//Stereo Comb Class
stereoComb::stereoComb()
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
    
    //Initial val.
    thisstereoSpread = 23;
}

//Copy constructor.
stereoComb::stereoComb(const stereoComb& copy)
{
    combL = copy.combL;
    combR = copy.combR;
    bufcombL = copy.bufcombL;
    bufcombR = copy.bufcombR;
}

//Filling the vectors with the largest values.
//I used vectors while developing but now that I keep each buffer the same maximum size I will change them to arrays.
void stereoComb::setbuffers(int size)
{
    for(int i = 0; i<size; i++)
    {
        bufcombL.push_back(0.0f);
    }
    
    for(int i = 0; i<size+stereospread; i++)
    {
        bufcombR.push_back(0.0f);
    }
    
    //updating the slider value.
    tuningSlider.setRange(0, size);
    tuningSlider.setValue(size);
    
    //Giving the buffers to the comb objects for them to write to.
    combL.setbuffer(bufcombL.data(),size);
    combR.setbuffer(bufcombR.data(),size+stereospread);
}

//Basic interfacing to Jezar's comb object.
void stereoComb::mute()
{
    combL.mute();
    combR.mute();
}

void stereoComb::setdamp(float val)
{
    combL.setdamp(val);
    combR.setdamp(val);
}

void stereoComb::setfeedback(float val)
{
    combL.setfeedback(val);
    combR.setfeedback(val);
}

//Updating the buffersize every audio iteration incase.
//Note this is not changing the size of the vector but telling the comb filter when to loop back to 0 in the vector.
float stereoComb::processLeft(float val)
{
    combL.setbufsize(tuningSlider.getValue());
    return combL.process(val);
}

//Note that the right buffer has the stereo spreadvalue added to it.
float stereoComb::processRight(float val)
{
    combR.setbufsize(tuningSlider.getValue()+thisstereoSpread);
    return combR.process(val);
}

//Drawing the gui.
void stereoComb::resized()
{
    Rectangle<int> area(getLocalBounds());
    area.removeFromTop(20);
    area.reduce(10, 10);
    tuningSlider.setBounds(area);
}

void stereoComb::setStereoSpread(const int& val)
{
    thisstereoSpread = val;
}



//-------------------------------------------------------------
//Allpass Filter Class
//-------------------------------------------------------------
//Stereo Allpass Class
stereoAllpass::stereoAllpass()
{
    //Setting up guis.
    addAndMakeVisible(&tuningSlider);
    tuningSlider.addListener(this);
    tuningSlider.setSliderStyle(Slider::LinearBarVertical);
    tuningSlider.setRange(0.0, 1.0);
    tuningSlider.setValue(0.5);
    
    addAndMakeVisible(&sliderLabel);
    sliderLabel.setText("Allpass Filter", dontSendNotification);
    sliderLabel.attachToComponent(&tuningSlider, false);
}

//Copy constructor.
stereoAllpass::stereoAllpass(const stereoAllpass& copy)
{
    allpassL = copy.allpassL;
    allpassR = copy.allpassR;
    bufallpassL = copy.bufallpassL;
    bufallpassR = copy.bufallpassR;
}

//This function is the same as the comb filters function.
void stereoAllpass::setbuffers(int size)
{
    for(int i = 0; i<size; i++)
    {
        bufallpassL.push_back(0.0f);
    }
    
    for(int i = 0; i<size+stereospread; i++)
    {
        bufallpassR.push_back(0.0f);
    }
    
    tuningSlider.setRange(0, size);
    tuningSlider.setValue(size);
    
    allpassL.setbuffer(bufallpassL.data(),size);
    allpassR.setbuffer(bufallpassR.data(),size+stereospread);
}

void stereoAllpass::mute()
{
    allpassL.mute();
    allpassR.mute();
}

void stereoAllpass::setfeedback(float val)
{
    allpassL.setfeedback(val);
    allpassR.setfeedback(val);
}

//This function is the same as the comb filters function.
float stereoAllpass::processLeft(float val)
{
    allpassL.setbufsize(tuningSlider.getValue());
    return allpassL.process(val);
}
//This function is the same as the comb filters function.
float stereoAllpass::processRight(float val)
{
    allpassR.setbufsize(tuningSlider.getValue()+thisstereoSpread);
    return allpassR.process(val);
}

//This function is the same as the comb filters function.
void stereoAllpass::resized()
{
    Rectangle<int> area(getLocalBounds());
    area.removeFromTop(20);
    area.reduce(10, 10);
    tuningSlider.setBounds(area);
}

//This function is the same as the comb filters function.
void stereoAllpass::setStereoSpread(const int& val)
{
    thisstereoSpread = val;
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
        //Each comb's buffer is set to 1 second at the largest
        combs.push_back(new stereoComb());
        combs[i]->setbuffers(44100);
        //Gui:
        addAndMakeVisible(combs[i]);
    }
    
    for(int i = 0; i<numallpasses; i++)
    {
        allpasses.push_back(new stereoAllpass());
        allpasses[i]->setbuffers(44100);
        allpasses[i]->setfeedback(0.5f);
        addAndMakeVisible(allpasses[i]);
    }
    
    //Initialise the unit to the original settings of Jezars
    setOriginalParameters();
    
    //Gui:
    addAndMakeVisible(&stereoSpreadSlider);
    stereoSpreadSlider.addListener(this);
    stereoSpreadSlider.setSliderStyle(Slider::LinearBarVertical);
    stereoSpreadSlider.setRange(0, 100);
    stereoSpreadSlider.setValue(23);
    
    addAndMakeVisible(&stereoSpreadSliderLabel);
    stereoSpreadSliderLabel.setText("Stereo Spread", dontSendNotification);
    stereoSpreadSliderLabel.attachToComponent(&stereoSpreadSlider, false);
    
    addAndMakeVisible(&originalParamsButton);
    originalParamsButton.setButtonText("Reset");
    originalParamsButton.addListener(this);
    
    
	setwet(initialwet);
	setroomsize(initialroom);
	setdry(initialdry);
	setdamp(initialdamp);
	setwidth(initialwidth);
	setmode(initialmode);

	// Buffer will be full of rubbish - so we MUST mute them
	mute();
}

//Reseting the untis settings to the original ones.
void revmodel::setOriginalParameters()
{
    for(int i = 0; i<numcombs; i++)
    {
        combs[i]->tuningSlider.setValue(combtuning[i]);
    }
    for(int i = 0; i<numallpasses; i++)
    {
        allpasses[i]->tuningSlider.setValue(allpasstuning[i]);
    }
    stereoSpreadSlider.setValue(23);
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
	for (int i=0;i<numallpasses;i++)
	{
		allpasses[i]->mute();
	}
}

//This loop is the loop that properly processes the current audio buffer.
void revmodel::processreplace(float *inputL, float *inputR, float *outputL, float *outputR, long numsamples, int skip)
{
	float outL,outR,input;

    //For all the samples in the buffer:
	while(numsamples-- > 0)
	{
		outL = outR = 0;
        //Take away any stereo and make input mono.
		input = (*inputL + *inputR) * gain;

		//From begining to end process the input through all the comb filters
		for(int i=0; i<numcombs; i++)
		{
            outL += combs[i]->processLeft(input);
            outR += combs[i]->processRight(input);
		}

        //From begining to end process the output of the comb filters through all the allpass filters.
        for(int i=0; i<numallpasses; i++)
		{
			outL = allpasses[i]->processLeft(outL);
			outR = allpasses[i]->processRight(outR);
		}

		// Re-write the output by combining the the Left and right comb filters and adding in the original input.
        //The wet1 and wet2 variables are decided by the "width" variable, it is basically a crossfade between comb filters.
		*outputL = outL*wet1 + outR*wet2 + *inputL*dry;
		*outputR = outR*wet1 + outL*wet2 + *inputR*dry;

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

	wet1 = wet*(width/2 + 0.5f);
	wet2 = wet*((1-width)/2);

	if (mode >= freezemode)
	{
		roomsize1 = 1;
		damp1 = 0;
		gain = muted;
	}
	else
	{
		roomsize1 = roomsize;
		damp1 = damp;
		gain = fixedgain;
	}

	for(i=0; i<numcombs; i++)
	{
		combs[i]->setfeedback(roomsize1);
	}

	for(i=0; i<numcombs; i++)
	{
		combs[i]->setdamp(damp1);
	}
    
    //I addd these two:
    for(i=0; i<numcombs; i++)
    {
        combs[i]->setStereoSpread(stereoSpreadSlider.getValue());
    }
    
    for(i=0; i<numallpasses; i++)
    {
        combs[i]->setStereoSpread(stereoSpreadSlider.getValue());
    }
}

// Jezar's comment:
// The following get/set functions are not inlined, because
// speed is never an issue when calling them, and also
// because as you develop the reverb model, you may
// wish to take dynamic action when they are called.
void revmodel::setroomsize(float value)
{
	roomsize = (value*scaleroom) + offsetroom;
	update();
}
float revmodel::getroomsize()
{
	return (roomsize-offsetroom)/scaleroom;
}
void revmodel::setdamp(float value)
{
	damp = value*scaledamp;
	update();
}
float revmodel::getdamp()
{
	return damp/scaledamp;
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
void revmodel::setwidth(float value)
{
	width = value;
	update();
}
float revmodel::getwidth()
{
	return width;
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
    stereoSpreadSlider.setBounds(areaextra);
    
    Rectangle<int> areaCombs(area);
    areaCombs.removeFromBottom(area.getHeight()/2);
    int sliderWidth = (areaCombs.getWidth()/numcombs) ;
    for(int i = 0; i<numcombs; i++)
    {
        combs[i]->setBounds(areaCombs.removeFromLeft(sliderWidth));
    }
    
    Rectangle<int> areaAllpass(area);
    areaAllpass.removeFromTop(area.getHeight()/2);
    sliderWidth = (areaAllpass.getWidth()/numallpasses);
    for(int i = 0; i<numallpasses; i++)
    {
        allpasses[i]->setBounds(areaAllpass.removeFromLeft(sliderWidth));
    }
}

//ends
