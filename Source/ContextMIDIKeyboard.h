/*
  ==============================================================================

    ContextMIDIKeyboard.h
    Created: 12 May 2023 6:23:05pm
    Author:  viktor

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MusicMath.h"

//==============================================================================
/*
*/
class ContextMIDIKeyboard  : public juce::MidiKeyboardComponent
{
public:
    ContextMIDIKeyboard(juce::MidiKeyboardState&, juce::KeyboardComponentBase::Orientation);
    ~ContextMIDIKeyboard();

    void drawWhiteNote(int midiNoteNumber, juce::Graphics& g, juce::Rectangle< float > area, bool isDown, bool isOver, juce::Colour lineColour, juce::Colour textColour) override;

    MusicMath mathObj;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ContextMIDIKeyboard)
};
