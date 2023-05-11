#include "MainComponent.h"
#include "IncomingMessageCallback.h"


//==============================================================================
MainComponent::MainComponent() : keyboardComponent(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard), 
startTime (juce::Time::getMillisecondCounterHiRes() * 0.001) 
{
    setSize(1024, 800);
    
    midiInputList.setTextWhenNoChoicesAvailable("No MIDI Inputs Enabled");
    midiOutputList.setTextWhenNoChoicesAvailable("No MIDI Output Enabled");
    currentProgramNr = defaultProgramNr;
    programNrLabel.setText("Program: " + juce::String(currentProgramNr), juce::dontSendNotification);

    keyLabel.setText(mathObj.DisplayKeys(currentKey), juce::dontSendNotification);
    modeLabel.setText(mathObj.DisplayModes(currentModeIndex), juce::dontSendNotification);

    auto midiInputs = juce::MidiInput::getAvailableDevices();
    auto midiOutputs = juce::MidiOutput::getAvailableDevices();
    juce::StringArray midiInputNames;
    juce::StringArray midiOutputNames;
    juce::MidiMessage defaultPrgMessage = juce::MidiMessage::programChange(1, currentProgramNr);

    allNotesOffMsg = juce::MidiMessage::allNotesOff(1);

    for (auto input : midiInputs)
        midiInputNames.add(input.name);

    for (auto output : midiOutputs)
        midiOutputNames.add(output.name);

    midiInputList.addItemList(midiInputNames, 1);
    midiInputList.onChange = [this] { setMidiInput(midiInputList.getSelectedItemIndex()); };
    midiOutputList.addItemList(midiOutputNames, 1);
    midiOutputList.onChange = [this, defaultPrgMessage] { midiOutput = juce::MidiOutput::openDevice(midiOutputList.getSelectedItemIndex()); midiOutput->sendMessageNow(defaultPrgMessage); };

    if (midiInputList.getNumItems() > 0)
    {   
        setMidiInput(0);
    }

    if (midiOutputList.getNumItems() > 0)
    {
        midiOutputList.setSelectedItemIndex(0);
        midiOutput = juce::MidiOutput::openDevice(0);
        midiOutput->sendMessageNow(defaultPrgMessage);
    }
    
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
    
    midiFileSave.setButtonText("Save .mid");
    midiFileSave.setName("btnSaveMid");
    midiFileSave.addListener(this);
    midiMessagesBox.setMultiLine(true, false);

    addAndMakeVisible(keyboardComponent);
    addAndMakeVisible(midiInputList);
    addAndMakeVisible(midiOutputList);
    addAndMakeVisible(midiMessagesBox);
    addAndMakeVisible(midiFileSave);
    addAndMakeVisible(programNrLabel);
    addAndMakeVisible(keyLabel);
    addAndMakeVisible(modeLabel);
    keyboardState.addListener(this);
}

void MainComponent::handleNoteOn(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    if (!isAddingFromMidiInput)
    {
        auto m = juce::MidiMessage::noteOn(midiChannel, midiNoteNumber, velocity);
        m.setTimeStamp(juce::Time::getMillisecondCounterHiRes() * 0.001);
        postMessageToList(m, "On-Screen Keyboard");
    }
    
}
void MainComponent::handleNoteOff(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float /*velocity*/)
{
    if (!isAddingFromMidiInput)
    {
        auto m = juce::MidiMessage::noteOff(midiChannel, midiNoteNumber);
        m.setTimeStamp(juce::Time::getMillisecondCounterHiRes() * 0.001);
        postMessageToList(m, "On-Screen Keyboard");
    }
}

void MainComponent::buttonClicked(juce::Button* button)
{
    if (button->getName() == "btnSaveMid")
    {
        juce::MidiFile midiFile;
        midiMessagesSequence.updateMatchedPairs();
        midiMessagesSequence.sort();
        midiFile.addTrack(midiMessagesSequence);
        
        midiFile.setSmpteTimeFormat(25, 40);
        
        juce::File realFile("C:/Data/midi_files/generated/test.mid");
        juce::FileOutputStream stream(realFile);

        if (midiFile.writeTo(stream, 0)) {
            std::cout << "WriteTo method->true\n";
        }
        else {
            std::cout << "WriteTo method->false\n";
        }
        stream.flush();
        midiMessagesSequence.clear();
    }
}

void MainComponent::postMessageToList(const juce::MidiMessage& message, const juce::String& source)
{
    int roleIndex = 0;
    int modeOffset = 0;
    int noteOffset = 0;
    int octave = 0;
    juce::String additionalInfo;

    if (message.isController())
    {
        additionalInfo = "Controller " + juce::String(message.getControllerNumber()) + " -> " + juce::String(message.getControllerValue());
        (new IncomingMessageCallback(this, message, source, additionalInfo))->post();
    }
    else if (!juce::MidiMessage::isMidiNoteBlack(message.getNoteNumber()))
    {
        octave = message.getNoteNumber() / 12;
        juce::MidiMessage modifiedMsg = message;
        modifiedMsg.setVelocity(1.0);
        roleIndex = mathObj.TranslateRoleIndex(message);
        noteOffset = mathObj.TranslateRoleToModeOffset(roleIndex, currentModeIndex);
        modeOffset = mathObj.TranslateRoleToModeOffset(currentModeIndex, 0);

        modifiedMsg.setNoteNumber(octave * 12 + currentKey + modeOffset + noteOffset);
        midiOutput->sendMessageNow(modifiedMsg);
        additionalInfo = juce::String(message.getNoteNumber()) + " [" + message.getMidiNoteName(message.getNoteNumber(), true, true, 3) + "] -> " + juce::String(modifiedMsg.getNoteNumber()) + " [" + modifiedMsg.getMidiNoteName(modifiedMsg.getNoteNumber(), true, true, 3) + "] Role: " + juce::String(roleIndex + 1) + " Mode Offset: " + juce::String(modeOffset) + " Note Offset: " + juce::String(noteOffset);
        (new IncomingMessageCallback(this, modifiedMsg, source, additionalInfo))->post();
    }
}

void MainComponent::addMessageToList(const juce::MidiMessage& message, const juce::String& source, juce::String& additionalInfo)
{
    //auto time = message.getTimeStamp() - startTime;
    //auto velocity = message.getVelocity();

    //auto hours = ((int)(time / 3600.0)) % 24;
    //auto minutes = ((int)(time / 60.0)) % 60;
    //auto seconds = ((int)time) % 60;
    //auto millis = ((int)(time * 1000.0)) % 1000;

    //auto timecode = juce::String::formatted("%02d:%02d:%02d.%03d",
    //    hours,
    //    minutes,
    //    seconds,
    //    millis);

    auto description = getMidiMessageDescription(message);

    //juce::String midiMessageString(timecode + "  -  " + description + " (" + source + ")"); // [7]
    midiMessagesSequence.addEvent(message, 0.0);
    
    if(midiMessagesSequence.getNumEvents()>200)
    {
        midiMessagesSequence.clear();
    }
    midiMessagesBox.setText(additionalInfo);
    repaint();
}

void MainComponent::setMidiInput(int index)
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

void MainComponent::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
    const juce::ScopedValueSetter<bool> scopedInputFlag(isAddingFromMidiInput, true);
    keyboardState.processNextMidiEvent(message);
    postMessageToList(message, source->getName());
}

juce::String MainComponent::getMidiMessageDescription(const juce::MidiMessage& m)
{
    if (m.isNoteOn())           return "Note on " + juce::String(m.getNoteNumber()) + " : "  + juce::MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3);
    if (m.isNoteOff())          return "Note off " + juce::String(m.getNoteNumber()) + " : " + juce::MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3);
    if (m.isProgramChange())    return "Program change " + juce::String(m.getProgramChangeNumber());
    if (m.isPitchWheel())       return "Pitch wheel " + juce::String(m.getPitchWheelValue());
    if (m.isAftertouch())       return "After touch " + juce::MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3) + ": " + juce::String(m.getAfterTouchValue());
    if (m.isChannelPressure())  return "Channel pressure " + juce::String(m.getChannelPressureValue());
    if (m.isAllNotesOff())      return "All notes off";
    if (m.isAllSoundOff())      return "All sound off";
    if (m.isMetaEvent())        return "Meta event";

    if (m.isController())
    {
        juce::String name("[" + juce::String(m.getControllerNumber()) + "]");
        
        switch (m.getControllerNumber())
        {
            case 1:
            {
                currentProgramNr = m.getControllerValue();
                auto prgChgMsg = juce::MidiMessage::programChange(1, currentProgramNr);
                programNrLabel.setText("Program: " + juce::String(currentProgramNr), juce::dontSendNotification);
                midiOutput->sendMessageNow(prgChgMsg);
                repaint();
                break;
            }
            case 2:
            {
                int value = m.getControllerValue();
                currentKeyIndex = (value / 128.0) * 12;
                if (currentKey != mathObj.TranslateKeyIndex(currentKeyIndex))
                {
                    currentKey = mathObj.TranslateKeyIndex(currentKeyIndex);

                    keyLabel.setText(mathObj.DisplayKeys(currentKey), juce::dontSendNotification);
                    repaint();
                    midiOutput->sendMessageNow(this->allNotesOffMsg);
                }
                break;
            }
            case 3:
            {
                int value = (m.getControllerValue() / 128.0) * 7;
                if (value != currentModeIndex)
                {
                    currentModeIndex = value;
                    modeLabel.setText(mathObj.DisplayModes(currentModeIndex), juce::dontSendNotification);
                    repaint();
                    midiOutput->sendMessageNow(this->allNotesOffMsg);
                }
                break;
            }
        default:
            return "Controller " + name + ": " + juce::String(m.getControllerValue());
            break;
        }
        
    }

    return juce::String::toHexString(m.getRawData(), m.getRawDataSize());
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    midiInputList.setBounds(0, 0, this->getWidth(), 25);
    keyboardComponent.setBounds(0, 25, this->getWidth(), 100);
    midiOutputList.setBounds(0, 125, this->getWidth(), 25);
    midiFileSave.setBounds(this->getWidth() - 100, 150, 100, 25);

    programNrLabel.setBounds(0, 150, 100, 25);
    keyLabel.setBounds(100, 150, 200, 25);
    modeLabel.setBounds(315, 150, 500, 25);
    
    midiMessagesBox.setBounds(0, 200, this->getWidth(), this->getHeight()-200);
}
