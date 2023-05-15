/*
  ==============================================================================

    ContextMIDIComponent.h
    Created: 11 May 2023 8:32:44pm
    Author:  viktor

  ==============================================================================
*/
#include <JuceHeader.h>
#include "MusicMath.h"
#include "KeyButton.h"
#include "ModeButton.h"
#include "ContextMIDIKeyboard.h"

#pragma once
class ContextMIDIComponent : public juce::Component, public juce::MidiInputCallback, public juce::MidiKeyboardStateListener, public juce::KeyListener, public juce::Timer
{
public:
    ContextMIDIComponent();
    ~ContextMIDIComponent();

    juce::AudioDeviceManager deviceManager;    

    juce::ComboBox midiInputList;                    
    juce::ComboBox midiOutputList;                   

    juce::Label programNrLabel;
    juce::Label keyLabel;
    juce::Label modeLabel;
    juce::Label midiInputLabel;
    juce::Label midiOutputLabel;
    juce::Label chordNameLabel;
    juce::Label chordModeLabel;

    juce::TextEditor midiMessagesBox;

    juce::MidiMessage allNotesOffMsg;
    juce::MidiMessage defaultPrgMessage;
    juce::MidiKeyboardState keyboardState;
    ContextMIDIKeyboard keyboardComponent;
    juce::MidiMessageSequence midiMessagesSequence;
    std::unique_ptr<juce::MidiOutput> midiOutput;
    juce::Array<juce::MidiDeviceInfo> midiInputs;
    juce::Array<juce::MidiDeviceInfo> midiOutputs;

    juce::OwnedArray<KeyButton> keysButtonsArray;
    juce::OwnedArray<ModeButton> modesButtonsArray;

    int lastInputIndex = 0;
    bool isAddingFromMidiInput = false;
    juce::int64 startTimestamp;
    int defaultProgramNr = 67; //70
    int currentProgramNr = 67;
    int currentKey = 0;
    int currentKeyIndex = 0;
    int currentModeIndex = 0;

    MusicMath mathObj;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void timerCallback() override;
    void resized() override;
    void setMidiInput(int);
    void updateGUI();
    void handleIncomingMidiMessage(juce::MidiInput*, const juce::MidiMessage&) override;
    void handleNoteOn(juce::MidiKeyboardState*, int, int, float) override;
    void handleNoteOff(juce::MidiKeyboardState*, int, int, float) override;
    void processMidiMessage(const juce::MidiMessage&, const juce::String&);
    void displayMessageInfo(juce::MidiMessage&, const juce::String&, juce::String&);
    void createContextButtons();
    void initGUIElements();
    void keyChanged();
    void modeChanged();
    void ProgramChanged();
    void SendMessageToMidiOut(juce::MidiMessage& message);
    void SendMessageToMidiSequence(juce::MidiMessage& message);
    juce::String getMidiMessageDescription(const juce::MidiMessage&);
    //==============================================================================
/** Called to indicate that a key has been pressed.

    If your implementation returns true, then the key event is considered to have
    been consumed, and will not be passed on to any other components. If it returns
    false, then the key will be passed to other components that might want to use it.

    @param key                      the keystroke, including modifier keys
    @param originatingComponent     the component that received the key event
    @see keyStateChanged, Component::keyPressed
*/
    bool keyPressed(const juce::KeyPress& key, juce::Component* originatingComponent) override;
    bool keyStateChanged(bool isKeyDown, juce::Component* originatingComponent) override;

private:
    //==============================================================================


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ContextMIDIComponent)
};