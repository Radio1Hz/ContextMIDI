/*
  ==============================================================================

    ContextMIDIComponent.cpp
    Created: 11 May 2023 8:32:44pm
    Author:  viktor

  ==============================================================================
*/

#include "ContextMIDIComponent.h"
#include "IncomingMessageCallback.h"
#include "KeyButton.h"
#include "ContextMIDIKeyboard.h"

ContextMIDIComponent::ContextMIDIComponent() : keyboardComponent(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),
startTimestamp(juce::Time::currentTimeMillis())
{
    currentProgramNr = defaultProgramNr;
    allNotesOffMsg = juce::MidiMessage::allNotesOff(1);
    defaultPrgMessage = juce::MidiMessage::programChange(1, currentProgramNr);
    midiMessagesSequence.clear();

    addAndMakeVisible(keyboardComponent);
    addAndMakeVisible(midiInputList);
    addAndMakeVisible(midiOutputList);
    addAndMakeVisible(midiMessagesBox);
    addAndMakeVisible(programNrLabel);
    addAndMakeVisible(keyLabel);
    addAndMakeVisible(modeLabel);
    addAndMakeVisible(midiInputLabel);
    addAndMakeVisible(midiOutputLabel);
    addAndMakeVisible(chordNameLabel);
    addAndMakeVisible(chordModeLabel);

    initGUIElements();
    startTimer(100);
}

ContextMIDIComponent::~ContextMIDIComponent()
{
}

void ContextMIDIComponent::initGUIElements()
{
    juce::StringArray midiInputNames;
    juce::StringArray midiOutputNames;

    //STATIC ELEMENTS INIT - DONE ONLY ONCE ----------------------------------------------------
    midiInputs = juce::MidiInput::getAvailableDevices();
    midiOutputs = juce::MidiOutput::getAvailableDevices();

    midiInputLabel.setText("Midi In:", juce::dontSendNotification);
    midiOutputLabel.setText("Midi Out:", juce::dontSendNotification);
    midiInputList.setTextWhenNoChoicesAvailable("No MIDI Inputs Enabled");
    midiOutputList.setTextWhenNoChoicesAvailable("No MIDI Output Enabled");
    chordNameLabel.setJustificationType(juce::Justification::centred);
    chordModeLabel.setJustificationType(juce::Justification::centred);
    chordNameLabel.setFont(juce::Font(40.0f, 0));
    chordModeLabel.setFont(juce::Font(12.0f, 0));

    for (auto input : midiInputs)
        midiInputNames.add(input.name);

    for (auto output : midiOutputs)
        midiOutputNames.add(output.name);

    midiInputList.addItemList(midiInputNames, 1);
    midiInputList.onChange = [this] {
        setMidiInput(midiInputList.getSelectedItemIndex());
    };

    midiOutputList.addItemList(midiOutputNames, 1);
    midiOutputList.onChange = [this] {
        midiOutput = juce::MidiOutput::openDevice(midiOutputs[midiOutputList.getSelectedItemIndex()].identifier);
        ProgramChanged();
    };

    // find the first enabled device and use that by default
    for (auto input : midiInputs)
    {
        if (deviceManager.isMidiInputDeviceEnabled(input.identifier))
        {
            setMidiInput(midiInputs.indexOf(input));
            break;
        }
    }

    // if no enabled devices were found just use the first one in the list
    if (midiInputList.getSelectedId() == 0)
        setMidiInput(0);

    if (midiOutputList.getNumItems() > 0)
    {
        midiOutputList.setSelectedItemIndex(0);
        midiOutput = juce::MidiOutput::openDevice(midiOutputs[0].identifier);
        juce::MidiDeviceInfo info = midiOutput->getDeviceInfo();
        SendMessageToMidiOut(defaultPrgMessage);
    }

    keyboardState.addListener(this);
    addKeyListener(this);

    createContextButtons();

    //DYNAMIC ELEMENTS INIT --------------------------------------------------------------------
    programNrLabel.setText("Program: " + juce::String(currentProgramNr), juce::dontSendNotification);
    modeLabel.setText(mathObj.DisplayModes(currentModeIndex), juce::dontSendNotification);
}
void ContextMIDIComponent::updateGUI()
{
    programNrLabel.setText("Program: " + juce::String(currentProgramNr), juce::dontSendNotification);
    modeLabel.setText(mathObj.DisplayModes(currentModeIndex), juce::dontSendNotification);
    keyLabel.setText(mathObj.DisplayKeys(currentKey), juce::dontSendNotification);

    int index = 0;
    juce::String keyButtonName = "key_" + juce::String(currentKeyIndex);
    for (KeyButton* btn : keysButtonsArray)
    {
        if (btn->getName().equalsIgnoreCase(keyButtonName))
        {
            btn->setToggleState(true, juce::dontSendNotification);
        }
        else
        {
            btn->setToggleState(false, juce::dontSendNotification);
        }
        index++;
    }

    index = 0;
    juce::String modeButtonName = "mode_" + juce::String(currentModeIndex);
    for (ModeButton* btn : modesButtonsArray)
    {
        if (btn->getName().equalsIgnoreCase(modeButtonName))
        {
            btn->setToggleState(true, juce::dontSendNotification);
        }
        else
        {
            btn->setToggleState(false, juce::dontSendNotification);
        }
        index++;
    }

    modeLabel.setText(mathObj.DisplayModes(currentModeIndex), juce::dontSendNotification);
    chordNameLabel.setText(mathObj.GetChordName(currentKeyIndex, currentModeIndex), juce::dontSendNotification);
    chordModeLabel.setText(mathObj.GetModeName(currentModeIndex), juce::dontSendNotification);

    repaint();
}

void ContextMIDIComponent::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
    const juce::ScopedValueSetter<bool> scopedInputFlag(isAddingFromMidiInput, true);
    keyboardState.processNextMidiEvent(message);
    processMidiMessage(message, source->getName());

    if (message.isController())
    {
        switch (message.getControllerNumber())
        {
            case 1:
            {
                currentProgramNr = message.getControllerValue();
                ProgramChanged();
                break;
            }
            case 2:
            {
                int value = message.getControllerValue();
                value = juce::roundToInt((message.getControllerValue() / 127.0) * 11);
                if (value != currentKeyIndex)
                {
                    currentKeyIndex = value;
                    currentKey = mathObj.TranslateKeyIndex(currentKeyIndex);
                    keyChanged();
                }

                break;
            }
            case 3:
            {
                int value = juce::roundToInt((message.getControllerValue() / 127.0) * 6);
                if (value != currentModeIndex)
                {
                    currentModeIndex = value;
                    modeChanged();
                }
                break;
            }
            default:
                break;
        }
    }
}

void ContextMIDIComponent::handleNoteOn(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    if (!isAddingFromMidiInput)
    {
        auto m = juce::MidiMessage::noteOn(midiChannel, midiNoteNumber, velocity);
        m.setTimeStamp(juce::Time::getMillisecondCounterHiRes() * 0.001);
        processMidiMessage(m, "On-Screen Keyboard");
    }

}
void ContextMIDIComponent::handleNoteOff(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float /*velocity*/)
{
    if (!isAddingFromMidiInput)
    {
        auto m = juce::MidiMessage::noteOff(midiChannel, midiNoteNumber);
        m.setTimeStamp(juce::Time::getMillisecondCounterHiRes() * 0.001);
        processMidiMessage(m, "On-Screen Keyboard");
    }
}

void ContextMIDIComponent::processMidiMessage(const juce::MidiMessage& message, const juce::String& source)
{
    int roleIndex = 0;
    int modeOffset = 0;
    int noteOffset = 0;
    int octave = 0;
    juce::MidiMessage modifiedMsg = message;

    if (!juce::MidiMessage::isMidiNoteBlack(message.getNoteNumber()) && !message.isController())
    {
        octave = message.getNoteNumber() / 12;
        
        modifiedMsg.setVelocity(1.0);
        roleIndex = mathObj.TranslateRoleIndex(message);
        noteOffset = mathObj.TranslateRoleToModeOffset(roleIndex, currentModeIndex);
        modeOffset = mathObj.TranslateRoleToModeOffset(currentModeIndex, 0);

        modifiedMsg.setNoteNumber(octave * 12 + currentKey + modeOffset + noteOffset);

        if (modifiedMsg.isNoteOnOrOff())
        {
            SendMessageToMidiSequence(modifiedMsg);
        }
        
        SendMessageToMidiOut(modifiedMsg);        
    }
    juce::String additionalInfo = juce::String(message.getNoteNumber()) + " [" + message.getMidiNoteName(message.getNoteNumber(), true, true, 3) + "] -> " + juce::String(modifiedMsg.getNoteNumber()) + " [" + modifiedMsg.getMidiNoteName(modifiedMsg.getNoteNumber(), true, true, 3) + "] Role: " + juce::String(roleIndex + 1) + " Mode Offset: " + juce::String(modeOffset) + " Note Offset: " + juce::String(noteOffset);
    (new IncomingMessageCallback(this, modifiedMsg, source, additionalInfo))->post();
}

void ContextMIDIComponent::displayMessageInfo(juce::MidiMessage& message, const juce::String& /*source*/, juce::String& additionalInfo)
{
    auto description = getMidiMessageDescription(message);
    if (!message.isController())
    {
        description += ", Additional Info: " + additionalInfo;
    }
    midiMessagesBox.setText(description);
}

void ContextMIDIComponent::setMidiInput(int index)
{
    auto list = juce::MidiInput::getAvailableDevices();

    deviceManager.removeMidiInputDeviceCallback(list[lastInputIndex].identifier, this);

    auto newInput = list[index];

    if (!deviceManager.isMidiInputDeviceEnabled(newInput.identifier))
        deviceManager.setMidiInputDeviceEnabled(newInput.identifier, true);

    deviceManager.addMidiInputDeviceCallback(newInput.identifier, this);
    midiInputList.setSelectedId(index + 1, juce::dontSendNotification);

    lastInputIndex = index;
}

juce::String ContextMIDIComponent::getMidiMessageDescription(const juce::MidiMessage& m)
{
    if (m.isNoteOn())           return "Note on " + juce::String(m.getNoteNumber()) + " : " + juce::MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3);
    if (m.isNoteOff())          return "Note off " + juce::String(m.getNoteNumber()) + " : " + juce::MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3);
    if (m.isProgramChange())    return "Program change " + juce::String(m.getProgramChangeNumber());
    if (m.isPitchWheel())       return "Pitch wheel " + juce::String(m.getPitchWheelValue());
    if (m.isAftertouch())       return "After touch " + juce::MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3) + ": " + juce::String(m.getAfterTouchValue());
    if (m.isChannelPressure())  return "Channel pressure " + juce::String(m.getChannelPressureValue());
    if (m.isAllNotesOff())      return "All notes off";
    if (m.isAllSoundOff())      return "All sound off";
    if (m.isMetaEvent())        return "Meta event";
    if (m.isController())       return "ControllerNr: " + juce::String(m.getControllerNumber()) + ", Value: " + juce::String(m.getControllerValue());

    return juce::String::toHexString(m.getRawData(), m.getRawDataSize());
}

void ContextMIDIComponent::createContextButtons()
{
    std::vector<juce::String> keysVector = mathObj.GetKeys();
    int index = 0;
    for (juce::String str : keysVector)
    {
        KeyButton* keyButton = new KeyButton(index);

        keyButton->onClick = [this, index] {
            currentKeyIndex = index;
            currentKey = mathObj.TranslateKeyIndex(currentKeyIndex);
            keyChanged();
        };

        keysButtonsArray.add(keyButton);
        addAndMakeVisible(keyButton);
        index++;
    }

    std::vector<juce::String> modesVector = mathObj.GetModes();
    index = 0;
    for (juce::String str : modesVector)
    {
        ModeButton* modeButton = new ModeButton(index);

        modeButton->onClick = [this, index] {
            currentModeIndex = index;
            modeChanged();
        };

        modesButtonsArray.add(modeButton);
        addAndMakeVisible(modeButton);
        index++;
    }
}

void ContextMIDIComponent::SendMessageToMidiOut(juce::MidiMessage& message)
{
    if (message.isProgramChange() || message.isAllNotesOff())
    {
        keyboardState.allNotesOff(1);
    }
    midiOutput->sendMessageNow(message);
}

void ContextMIDIComponent::SendMessageToMidiSequence(juce::MidiMessage& message)
{
    message.setTimeStamp(mathObj.GetNumberOfTicksElapsed(startTimestamp));
    midiMessagesSequence.addEvent(message);
    midiMessagesSequence.updateMatchedPairs();
}

bool ContextMIDIComponent::keyPressed(const juce::KeyPress& key, juce::Component* /*originatingComponent*/)
{
    int modeIndexPrior = currentModeIndex;

    if (key == juce::KeyPress::F1Key)
    {
        currentKeyIndex = 0;
    }
    if (key == juce::KeyPress::F2Key)
    {
        currentKeyIndex = 1;
    }
    if (key == juce::KeyPress::F3Key)
    {
        currentKeyIndex = 2;
    }
    if (key == juce::KeyPress::F4Key)
    {
        currentKeyIndex = 3;
    }
    if (key == juce::KeyPress::F5Key)
    {
        currentKeyIndex = 4;
    }
    if (key == juce::KeyPress::F6Key)
    {
        currentKeyIndex = 5;
    }
    if (key == juce::KeyPress::F7Key)
    {
        currentKeyIndex = 6;
    }
    if (key == juce::KeyPress::F8Key)
    {
        currentKeyIndex = 7;
    }
    if (key == juce::KeyPress::F9Key)
    {
        currentKeyIndex = 8;
    }
    if (key == juce::KeyPress::F10Key)
    {
        currentKeyIndex = 9;
    }
    if (key == juce::KeyPress::F11Key)
    {
        currentKeyIndex = 10;
    }
    if (key == juce::KeyPress::F12Key)
    {
        currentKeyIndex = 11;
    }

    if (key == juce::KeyPress::rightKey)
    {
        currentKeyIndex = (++currentKeyIndex) % 12;
    }

    if (key == juce::KeyPress::leftKey)
    {
        currentKeyIndex--;
        if (currentKeyIndex < 0)currentKeyIndex = 11;
    }

    if (key == juce::KeyPress::upKey)
    {
        currentModeIndex = (++currentModeIndex) % 7;
    }

    if (key == juce::KeyPress::downKey)
    {
        currentModeIndex--;
        if (currentModeIndex < 0)currentModeIndex = 6;
    }

    if (key == juce::KeyPress::backspaceKey)
    {
        mathObj.saveMidiPattern(midiMessagesSequence);
        startTimestamp = juce::Time::currentTimeMillis();
        midiMessagesSequence.clear();
    }

    if (modeIndexPrior != currentModeIndex)
    {
        modeChanged();
    }

    if(currentKey != mathObj.TranslateKeyIndex(currentKeyIndex))
    {
        currentKey = mathObj.TranslateKeyIndex(currentKeyIndex);
        keyChanged();
    }
    
    return true;
};


void ContextMIDIComponent::keyChanged()
{
    SendMessageToMidiOut(allNotesOffMsg);
    SendMessageToMidiSequence(allNotesOffMsg);
}

void ContextMIDIComponent::modeChanged()
{
    SendMessageToMidiOut(allNotesOffMsg);
    SendMessageToMidiSequence(allNotesOffMsg);
}

void ContextMIDIComponent::ProgramChanged()
{
    auto prgChgMsg = juce::MidiMessage::programChange(1, currentProgramNr);
    prgChgMsg.setTimeStamp(mathObj.GetNumberOfTicksElapsed(startTimestamp));
    SendMessageToMidiOut(prgChgMsg);
    SendMessageToMidiSequence(prgChgMsg);
}

bool ContextMIDIComponent::keyStateChanged(bool isKeyDown, juce::Component* /*originatingComponent*/)
{
    return isKeyDown;
}

void ContextMIDIComponent::timerCallback()
{
    updateGUI();
}

void ContextMIDIComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void ContextMIDIComponent::resized()
{
    midiInputLabel.setBounds(0, 0, 65, 25);
    midiInputList.setBounds(65, 0, this->getWidth()/2 - 75, 25);
    midiOutputLabel.setBounds(this->getWidth() / 2, 0, 75, 25);
    midiOutputList.setBounds(this->getWidth() / 2 + 75, 0, this->getWidth()/2 - 85, 25);

    keyboardComponent.setBounds(0, 40, this->getWidth(), 100);

    programNrLabel.setBounds(0, 150, 100, 25);
    keyLabel.setBounds(100, 150, 200, 25);
    modeLabel.setBounds(315, 150, 500, 25);

    midiMessagesBox.setBounds(0, this->getHeight() - 25, this->getWidth(), 25);

    // Draw key buttons in circle
    int index = 0;
    int radius = 150;
    int x;
    int y;

    juce::Rectangle<int> keyBounds(0, 0, radius*2, radius*2);
    keyBounds.setCentre(this->getWidth()/2, this->getHeight()/2);
    keyBounds.setTop(300);
    juce::Point<int> centerPoint(keyBounds.getCentre());
    float PI = juce::MathConstants<float>::pi;

    for (KeyButton* btn : keysButtonsArray)
    {
        x = juce::roundToInt(centerPoint.getX() + radius * std::sin(2 * PI * index / 12.0f));
        y = juce::roundToInt(centerPoint.getY() - radius * std::cos(2 * PI * index / 12.0f));

        juce::Rectangle<int> currentButtonRect(x - btn->size/2, y - btn->size / 2, btn->size, btn->size);
        btn->setBounds(currentButtonRect);
        index++;
    }

    // Draw mode buttons in circle
    index = 0;
    radius = 80;

    for (ModeButton* btn : modesButtonsArray)
    {
        x = juce::roundToInt(centerPoint.getX() + radius * std::sin(2 * PI * index / 7.0f));
        y = juce::roundToInt(centerPoint.getY() - radius * std::cos(2 * PI * index / 7.0f));

        juce::Rectangle<int> currentButtonRect(x - btn->size / 2, y - btn->size / 2, btn->size, btn->size);
        btn->setBounds(currentButtonRect);
        index++;
    }

    chordNameLabel.setBounds(keyBounds);
    keyBounds.translate(0, 25);
    chordModeLabel.setBounds(keyBounds);
}