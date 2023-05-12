/*
  ==============================================================================

    ModeButton.h
    Created: 12 May 2023 7:22:01am
    Author:  viktor

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "MusicMath.h"

class ModeButton : public juce::TextButton
{
public:
    //==============================================================================
    ModeButton(int modeIndex);
    ~ModeButton();

    void paint(juce::Graphics&) override;

    MusicMath mathObj;
    int size = 40;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModeButton)
};