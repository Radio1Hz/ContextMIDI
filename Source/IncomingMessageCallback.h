/*
  ==============================================================================

    IncomingMessageCallback.h
    Created: 9 May 2023 9:55:45am
    Author:  viktor

  ==============================================================================
*/
#include <JuceHeader.h>
#include "MainComponent.h"

#pragma once
class IncomingMessageCallback : public juce::CallbackMessage
{
public: 
    IncomingMessageCallback(MainComponent*, const juce::MidiMessage&,const juce::String&, juce::String&);
    ~IncomingMessageCallback();
    
    void messageCallback() override;

    MainComponent* _parent;
    juce::MidiMessage _message;
    juce::String _source;
    juce::String _additionalInfo;
};