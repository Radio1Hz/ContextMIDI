/*
  ==============================================================================

    IncomingMessageCallback.cpp
    Created: 9 May 2023 9:55:45am
    Author:  viktor

  ==============================================================================
*/

#include "MainComponent.h"
#include "IncomingMessageCallback.h"
#include "ContextMIDIComponent.h"

IncomingMessageCallback::IncomingMessageCallback(ContextMIDIComponent* parent, const juce::MidiMessage& message, const juce::String& source, juce::String& additionalInfo)
{
    this->_parent = parent;
    this->_message = message;
    this->_source = source;
    this->_additionalInfo = additionalInfo;
}

IncomingMessageCallback::~IncomingMessageCallback()
{

}

void IncomingMessageCallback::messageCallback()
{
    _parent->addMessageToList(this->_message, this->_source, this->_additionalInfo);   
}