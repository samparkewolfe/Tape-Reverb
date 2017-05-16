#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "revmodel.hpp"


class MainContentComponent   : public AudioAppComponent,
public Button::Listener,
public Slider::Listener

{
    //This is the reverb class that does all the processing.
    revmodel model;
    
    
    //Here are all the Gui's for interfacing with the reverb class front end.
    ToggleButton modeToggle;    //Freeze Mode
    Slider feedbackSlider,      //Room Size
    dampSlider,                 //Comb Dampening
    wetSlider,                  //Wet Mix
    drySlider;                  //Dry Mix
    Label feedbackLabel,
    dampLabel,
    wetLabel,
    dryLabel;
    GroupComponent group;
    
public:
    
    MainContentComponent();
    ~MainContentComponent();
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked(Button* button) override;
    void sliderValueChanged(Slider* slider) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
