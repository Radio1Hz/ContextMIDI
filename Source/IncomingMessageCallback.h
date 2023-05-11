/*
  ==============================================================================

    IncomingMessageCallback.h
    Created: 9 May 2023 9:55:45am
    Author:  viktor

  ==============================================================================
*/
#include <JuceHeader.h>
#include "MainComponent.h"
#include "ContextMIDIComponent.h"

#pragma once
class IncomingMessageCallback : public juce::CallbackMessage
{
public: 
    IncomingMessageCallback(ContextMIDIComponent*, const juce::MidiMessage&, const juce::String&, juce::String&);
    ~IncomingMessageCallback();
    
    void messageCallback() override;

    ContextMIDIComponent* _parent;
    juce::MidiMessage _message;
    juce::String _source;
    juce::String _additionalInfo;
};