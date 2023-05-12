/*
  ==============================================================================

    MusicMath.h
    Created: 10 May 2023 8:17:33am
    Author:  viktor

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>

class MusicMath 
{
public:
    MusicMath();
    ~MusicMath();
	juce::String DisplayKeys(int);
	juce::String DisplayModes(int);
	juce::String GetKeyName(int);
	juce::String GetModeName(int);
	juce::String GetModeDegree(int);
	juce::String GetChordName(int keyIndex, int modeIndex);
	std::vector<juce::String> GetKeys();
	std::vector<juce::String> GetModes();

	int TranslateKeyIndex(int);
	int TranslateRoleIndex(const juce::MidiMessage&);
	int TranslateRoleToModeOffset(int, int);

private:
    std::vector<int> _keys_offset = { 0, 7, 2, 9, 4, 11, 6, 1, 8, 3, 10, 5 };
	std::vector<juce::String> _keys_display = { "C", "G", "D", "A", "E", "B", "F#", "C#", "G#", "D#", "Bb", "F" };
	std::vector<std::vector<int>> _modes_offset {
											{0, 2, 4, 5, 7, 9, 11},
											{0, 2, 3, 5, 7, 9, 10},
											{0, 1, 3, 5, 7, 8, 10},
											{0, 2, 4, 6, 7, 9, 11},
											{0, 2, 4, 5, 7, 9, 10},
											{0, 2, 3, 5, 7, 8, 10},
											{0, 1, 3, 5, 6, 8, 10}
										};
	std::vector<juce::String> _modes_display = { "Ionian", "Dorian", "Phrygian", "Lydian", "Mixolydian", "Aeolian", "Locrian" };
	std::vector<juce::String> _modes_display_degrees = { "I", "II", "III", "IV", "V", "VI", "VII" };
};
//int realNoteNumber = _keys_offset[keyIndex] + _modes_offset[0][modeIndex];
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

Octave

Keys: (Base)
Display
C	G	D	A	E	B	F♯	C♯	G♯	D♯	B♭	F
Math
0	7	2	9	4	11	6	1	8	3	10	5

Modes:
Display
I, II, III, IV, V, VI, VII
Math
	0	1	2	3	4	5	6	7	8	9	10	11
	----------------------------------------------
I	o	.	o	.	o	o	.	o	.	o	.	o
II	o	.	o	o	.	o	.	o	.	o	o	.
III	o	o	.	o	.	o	.	o	o	.	o	.
IV	o	.	o	.	o	.	o	o	.	o	.	o
V	o	.	o	.	o	o	.	o	.	o	o	.
VI	o	.	o	o	.	o	.	o	o	.	o	.
VII	o	o	.	o	.	o	o	.	o	.	o	.

Degrees:
Display
1, 2, 3, 4, 5, 6, 7
Math
	0	1	2	3	4	5	6	7	8	9	10	11
	----------------------------------------------
I	0	.	2	.	4	5	.	7	.	9	.	11
II	0	.	2	3	.	5	.	7	.	9	10	.
III	0	1	.	3	.	5	.	7	8	.	10	.
IV	0	.	2	.	4	.	6	7	.	9	.	11
V	0	.	2	.	4	5	.	7	.	9	10	.
VI	0	.	2	3	.	5	.	7	8	.	10	.
VII	0	1	.	3	.	5	6	.	8	.	10	.

	1	2	3	4	5	6	7
	----------------------------------------------
I	0	2	4	5	7	9	11
II	0	2	3	5	7	9	10
III	0	1	3	5	7	8	10
IV	0	2	4	6	7	9	11
V	0	2	4	5	7	9	10
VI	0	2	3	5	7	8	10
VII	0	1	3	5	6	8	10


*/