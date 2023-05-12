/*
  ==============================================================================

    KeyButton.h
    Created: 12 May 2023 7:21:52am
    Author:  viktor

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "MusicMath.h"

class KeyButton : public juce::TextButton
{
public:
    //==============================================================================
    KeyButton(int keyIndex);
    ~KeyButton();

    void paint(juce::Graphics&) override;

    MusicMath mathObj;
    int size = 60;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KeyButton)
};