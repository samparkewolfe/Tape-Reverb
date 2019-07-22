/*
 ==============================================================================
 
 This file was auto-generated!
 
 ==============================================================================
 */

#include "MainComponent.h"

MainContentComponent::MainContentComponent()
{
    
    setSize (400, 400);
    
    // specify the number of input and output channels that we want to open
    setAudioChannels (2, 2);
    
    //Add the gui of medel to this component.
    addAndMakeVisible(&model);
    
    //Initialise all the other guis for front end.
    addAndMakeVisible(&group);
    group.setText("Reverb Front End");
    group.setColour(GroupComponent::outlineColourId, Colours::grey);
    group.toFront(false);
    
    addAndMakeVisible(&modeToggle);
    modeToggle.addListener(this);
    modeToggle.setButtonText("Freeze");
    
    addAndMakeVisible(&feedbackSlider);
    feedbackSlider.addListener(this);
    feedbackSlider.setSliderStyle(Slider::LinearBarVertical);
    feedbackSlider.setRange(0.0, 1.0);
    feedbackSlider.setValue(0.5);
    
    addAndMakeVisible(&wetSlider);
    wetSlider.addListener(this);
    wetSlider.setSliderStyle(Slider::LinearBarVertical);
    wetSlider.setRange(0.0, 1.0);
    wetSlider.setValue(0.5);
    
    addAndMakeVisible(&drySlider);
    drySlider.addListener(this);
    drySlider.setSliderStyle(Slider::LinearBarVertical);
    drySlider.setRange(0.0, 1.0);
    drySlider.setValue(0.5);
    
    addAndMakeVisible(&feedbackLabel);
    feedbackLabel.setText("Feed Back", dontSendNotification);
    feedbackLabel.attachToComponent (&feedbackSlider, false);
    
    addAndMakeVisible(&wetLabel);
    wetLabel.setText("Wet", dontSendNotification);
    wetLabel.attachToComponent (&wetSlider, false);
    
    addAndMakeVisible(&dryLabel);
    dryLabel.setText("Dry", dontSendNotification);
    dryLabel.attachToComponent (&drySlider, false);

}

MainContentComponent::~MainContentComponent()
{
    shutdownAudio();
}

void MainContentComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    
}

void MainContentComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    
    //Send the buffers to the reveb unit.
    float** inputs = bufferToFill.buffer->getArrayOfWritePointers();
    float** outputs = bufferToFill.buffer->getArrayOfWritePointers();
    
    model.processreplace(inputs[0],inputs[1],outputs[0],outputs[1],bufferToFill.numSamples,1);
}

void MainContentComponent::releaseResources()
{
    
}

void MainContentComponent::paint (Graphics& g)
{
    
    //g.fillAll (Colours::black);
    
}

void MainContentComponent::resized()
{
    //Draw the back end of the reverb.
    Rectangle<int> areaModel(getLocalBounds());
    areaModel.removeFromBottom(getHeight()/3);
    areaModel.reduce(10, 10);
    model.setBounds(areaModel);
    
    //Draw the front end of the reverb.
    Rectangle<int> areaSliders(getLocalBounds());
    areaSliders.removeFromTop(2*getHeight()/3);
    
    areaSliders.reduce(10, 10);
    group.setBounds(areaSliders);
    areaSliders.reduce(10, 10);
    areaSliders.removeFromTop(30);
    
    int sliderGap = 10;
    int sliderWidth = areaSliders.getWidth()/4 - 10;
    modeToggle.setBounds(areaSliders.removeFromLeft(sliderWidth));
    areaSliders.removeFromLeft(sliderGap);
    feedbackSlider.setBounds(areaSliders.removeFromLeft(sliderWidth));
    areaSliders.removeFromLeft(sliderGap);
    areaSliders.removeFromLeft(sliderGap);
    wetSlider.setBounds(areaSliders.removeFromLeft(sliderWidth));
    areaSliders.removeFromLeft(sliderGap);
    drySlider.setBounds(areaSliders.removeFromLeft(sliderWidth));
}

void MainContentComponent::buttonClicked(juce::Button *button)
{
    //Updating the button.
    if(button == &modeToggle)   model.setmode(button->getToggleState());
}

void MainContentComponent::sliderValueChanged(juce::Slider *slider)
{
    //Updating the sliders.
    if(slider == & feedbackSlider)  model.setfeedback(slider->getValue());
    if(slider == & wetSlider)       model.setwet(slider->getValue());
    if(slider == & drySlider)       model.setdry(slider->getValue());
}
