/*
 ==============================================================================
 
 This file was auto-generated!
 
 ==============================================================================
 */

#include "MainComponent.h"

MainContentComponent::MainContentComponent()
{
    setSize (800, 600);
    setAudioChannels (2, 2);
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
    
    bufferToFill.clearActiveBufferRegion();
}

void MainContentComponent::releaseResources()
{
    
}

void MainContentComponent::paint (Graphics& g)
{
    
    g.fillAll (Colours::black);
    
}

void MainContentComponent::resized()
{
    
}
