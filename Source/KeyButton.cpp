/*
  ==============================================================================

    KeyButton.cpp
    Created: 12 May 2023 7:21:52am
    Author:  viktor

  ==============================================================================
*/

#include "KeyButton.h"
#include "MusicMath.h"

KeyButton::KeyButton(int keyIndex)
{
    this->keyIndex = keyIndex;
    this->setButtonText(mathObj.GetKeyName(keyIndex));
    this->setName("key_" + juce::String(keyIndex));
    this->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::darkcyan);
    this->setSize(size, size);
}

KeyButton::~KeyButton()
{

}

void KeyButton::paint(juce::Graphics& g)
{
    juce::Rectangle<float> rect(0,0, size * 1.0f, size * 1.0f);
    g.setColour(mathObj.GetKeyColour(this->keyIndex));
    g.fillEllipse(rect);
    
    if (this->getToggleState())
    {   
        rect.expand(-2.0f, -2.0f);
        g.setColour(juce::Colours::white);
        g.drawEllipse(rect, 4.0);
    }
    g.setFont(18);
    g.setColour(juce::Colours::white);
    g.drawText(this->getButtonText(), rect, juce::Justification::centred);
}