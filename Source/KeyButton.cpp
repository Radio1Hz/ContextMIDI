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
    g.setColour(juce::Colour::fromRGBA(25, 25, 25, 128));
    if (this->getToggleState())
    {
        g.setColour(juce::Colour::fromRGBA(165, 6, 189, 128));
    }
    g.fillEllipse(rect);
    g.setColour(juce::Colours::lightgrey);
    g.drawText(this->getButtonText(), rect, juce::Justification::centred);
}