#pragma once

#include <JuceHeader.h>
#include "MusicMath.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component, public juce::MidiInputCallback, public juce::MidiKeyboardStateListener, public juce::Button::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    juce::AudioDeviceManager deviceManager;           // [1]
    juce::ComboBox midiInputList;                     // [2]
    
    juce::ComboBox midiOutputList;                     // [2]
    
    juce::Label programNrLabel;
    juce::Label keyLabel;
    juce::Label modeLabel;
    int lastInputIndex = 0;                           // [3]
    bool isAddingFromMidiInput = false;               // [4]

    juce::MidiKeyboardState keyboardState;            // [5]
    juce::MidiKeyboardComponent keyboardComponent;    // [6]

    juce::TextEditor midiMessagesBox;
    juce::MidiMessageSequence midiMessagesSequence;
    juce::TextButton midiFileSave;
    std::unique_ptr<juce::MidiOutput> midiOutput;

    double startTime;
    int defaultProgramNr = 67; //70
    int currentProgramNr;
    int currentKey = 0;
    int currentKeyIndex = 0;
    int currentModeIndex = 0;

    MusicMath mathObj;
    juce::MidiMessage allNotesOffMsg;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void setMidiInput(int);
    void handleIncomingMidiMessage(juce::MidiInput*, const juce::MidiMessage&) override;
    void handleNoteOn(juce::MidiKeyboardState*, int, int, float) override;
    void handleNoteOff(juce::MidiKeyboardState*, int, int, float) override;
    void postMessageToList(const juce::MidiMessage&, const juce::String&);
    void addMessageToList(const juce::MidiMessage&, const juce::String&, juce::String&);
    void buttonClicked(juce::Button*) override;
    juce::String getMidiMessageDescription(const juce::MidiMessage&);

private:
    //==============================================================================
    // Your private member variables go here...


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
