/*
  ==============================================================================

    ContextMIDIKeyboard.cpp
    Created: 12 May 2023 6:23:05pm
    Author:  viktor

  ==============================================================================
*/
//#pragma once
#include <JuceHeader.h>
#include "ContextMIDIKeyboard.h"
#include "ContextMIDIComponent.h"

//==============================================================================
ContextMIDIKeyboard::ContextMIDIKeyboard(juce::MidiKeyboardState& keyboardState, juce::KeyboardComponentBase::Orientation orientation) : juce::MidiKeyboardComponent(keyboardState, orientation)
{
    
}

ContextMIDIKeyboard::~ContextMIDIKeyboard()
{
}

void ContextMIDIKeyboard::drawWhiteNote(int midiNoteNumber, juce::Graphics& g, juce::Rectangle<float> area, bool isDown, bool /*isOver*/, juce::Colour /*lineColour*/, juce::Colour textColour)
{
    
    ContextMIDIComponent* parent = dynamic_cast<ContextMIDIComponent*> (getParentComponent());  
    int noteRoleIndex = mathObj.GetRoleByNoteNumber(midiNoteNumber);
    if (noteRoleIndex == 0) //ROOT
    {
        g.setColour(juce::Colour::fromRGBA(128, 0, 0, 50));
        g.fillRect(area);
    }
    if (noteRoleIndex == 4) //FIFTH
    {
        g.setColour(juce::Colour::fromRGBA(0, 128, 0, 50));
        g.fillRect(area);
    }
    if (isDown)
    {
        g.setColour(juce::Colour::fromRGBA(128, 128, 0, 75));
        g.fillRect(area);
    }
    juce::Rectangle drawingRect = area;
    drawingRect.setHeight(15);
    drawingRect.setBottom(area.getBottom());
    g.setColour(textColour);
    g.setFont(11);
    
    g.drawText(juce::String(noteRoleIndex + 1), drawingRect, juce::Justification::centredBottom);
    drawingRect.translate(0, -15);
    g.drawText(mathObj.GetNoteName(noteRoleIndex, parent->currentKeyIndex, parent->currentModeIndex), drawingRect, juce::Justification::centredBottom);
    juce::Line<float> line(area.getTopRight(), area.getBottomRight());
    g.drawLine(line, 1.0);
}
