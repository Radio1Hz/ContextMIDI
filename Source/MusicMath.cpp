/*
  ==============================================================================

    MusicMath.cpp
    Created: 10 May 2023 8:17:33am
    Author:  viktor

  ==============================================================================
*/

#include "MusicMath.h"
MusicMath::MusicMath()
{

}

MusicMath::~MusicMath()
{

}

juce::String MusicMath::DisplayKeys(int currentKey)
{
    juce::String out = "";
    for (int i = 0; i < 12; i++)
    {
        if (currentKey == _keys_offset[i])
        {
            out += "[" + _keys_display[i] += "] ";
        }
        else
        {
            out += _keys_display[i] + " ";
        }
    }
    return out;
}

juce::String MusicMath::DisplayModes(int currentModeIndex)
{
    juce::String out = "";
    for (int i = 0; i < 7; i++)
    {
        if (currentModeIndex == i)
        {
            out += "[" + _modes_display[i] + "] ";
        }
        else
        {
            out += _modes_display[i] + " ";
        }
        
    }
    return out;
}

std::vector<juce::String> MusicMath::GetKeys()
{
    std::vector<juce::String> copyKeys = _keys_display;
    return copyKeys;
}

std::vector<juce::String> MusicMath::GetModes()
{
    std::vector<juce::String> copyModes = _modes_display_degrees;
    return copyModes;
}

int MusicMath::TranslateKeyIndex(int index)
{
    if (index > 11 || index < 0)
    {
        return 0;
    }
    else
    {
        return _keys_offset[index];
    }
    
}
int MusicMath::TranslateRoleToModeOffset(int roleIndex, int modeIndex)
{
    if (roleIndex < 0 || roleIndex > 6 || modeIndex < 0 || modeIndex > 6)
    {
        return -1;
    }
    
    return _modes_offset[modeIndex][roleIndex];
}

juce::String MusicMath::GetKeyName(int keyIndex)
{
    return _keys_display[keyIndex];
}

juce::String MusicMath::GetModeName(int modeIndex)
{
    return _modes_display[modeIndex];
}
juce::String MusicMath::GetModeDegree(int modeIndex)
{
    return _modes_display_degrees[modeIndex];
}
juce::String MusicMath::GetChordName(int keyIndex, int modeIndex)
{
    int realNoteNumber = _keys_offset[keyIndex] + _modes_offset[0][modeIndex];
    juce::String chordName = juce::MidiMessage::getMidiNoteName(realNoteNumber, true, false, 3);
    if (modeIndex == 1 || modeIndex == 2 || modeIndex == 5)
        chordName += "m";
    if (modeIndex == 6)
        chordName += "dim";
    return chordName;
}
// Input: MidiNoteNumber (60, 61, 62...) Output: Role in the scale (white keys index 0, 1, 2, 3 [root, second, third...]) 
// Returns -1 if black key
int MusicMath::TranslateRoleIndex(const juce::MidiMessage& midiNote) 
{
    int moduoNumber = midiNote.getNoteNumber() % 12;
    if (!juce::MidiMessage::isMidiNoteBlack(moduoNumber))
    {
        switch (moduoNumber)
        {
        case 0:
            return 0; // DO     - ROOT
            break;
        case 2:
            return 1; // RE     - SECOND
            break;
        case 4:
            return 2; // MI     - THIRD
            break;
        case 5:
            return 3; // FA     - FOURTH
            break;
        case 7:
            return 4; // SOL    - FIFTH
            break;
        case 9:
            return 5; // LA     - SIXTH
            break;
        case 11:
            return 6; // SI     - SEVENTH
            break;
        default:
            break;
        }
    }
    return -1;
}

/*
Octave	C	C#	D	D#	E	F	F#	G	G#	A	Bb	B
   -2	0	1	2	3	4	5	6	7	8	9	10	11
   -1	12	13	14	15	16	17	18	19	20	21	22	23
    0	24	25	26	27	28	29	30	31	32	33	34	35
    1	36	37	38	39	40	41	42	43	44	45	46	47
    2	48	49	50	51	52	53	54	55	56	57	58	59
    3	60	61	62	63	64	65	66	67	68	69	70	71
    4	72	73	74	75	76	77	78	79	80	81	82	83
    5	84	85	86	87	88	89	90	91	92	93	94	95
    6	96	97	98	99	100	101	102	103	104	104	106	107
    7	108	109	110	111	112	113	114	115	116	117	118	119
    8	120	121	122	123	124	125	126	127
*/