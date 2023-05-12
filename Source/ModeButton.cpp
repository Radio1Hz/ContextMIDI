/*
  ==============================================================================

    ModeButton.cpp
    Created: 12 May 2023 7:22:01am
    Author:  viktor

  ==============================================================================
*/

#include "ModeButton.h"
#include "MusicMath.h"

ModeButton::ModeButton(int modeIndex)
{
    this->setButtonText(mathObj.GetModeDegree(modeIndex));
    this->setName("mode_" + juce::String(modeIndex));
    this->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::darkcyan);
    this->setSize(size, size);
}

ModeButton::~ModeButton()
{

}

void ModeButton::paint(juce::Graphics& g)
{
    juce::Rectangle<float> rect(0, 0, size * 1.0f, size * 1.0f);
    g.setColour(juce::Colour::fromRGBA(0, 0, 0, 128));
    if (this->getToggleState())
    {
        g.setColour(juce::Colour::fromRGBA(255, 0, 0, 128));
    }
    g.fillEllipse(rect);
    g.setColour(juce::Colours::lightgrey);
    g.drawText(this->getButtonText(), rect, juce::Justification::centred);
}