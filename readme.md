# Context MIDI v1.1

Enables players to use white keys as roles in the current context.
White keys are used as roles in the current key and mode:

```text
"C" is used always as ROOT
"D" is used always as SECOND (major second in all keys/modes except Phrygian and Locrian - minor second then)
"E" is used always as THIRD (minor third in all keys/modes except Ionian, Lydian and Mixolydian - major third then)
"F" is used always as FOURTH (perfect fourth in all keys/modes except Lydian - augmented then)
"G" is used always as FIFTH (perfect fifth in all keys/modes except Locrian - augmented then)
"A" is used always as SIXTH (major sixth in all keys/modes except Phrygian, Aeolian and Locrian - minor then)
"B" is used always as SEVENTH (minor seventh in all keys/modes except Ionian and Lydian - major then)
```

![Screenshot](/Assets/ScreenshotV1.1.png)

```text
MIDI Control:
Ctrl 1: Changes Program
Ctrl 2: Changes Key
Crtl 3: Changes Mode

Keyboard Control:
Arrows <- -> change keys
```

Writen in JUCE C++ Framework.
Install Juicer and open ContextMIDI.juicer.

```text

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

Keys:
Display
C	G	D	A	E	B	F♯	C♯	G♯	D♯	B♭	F
Math - offset from C
0	7	2	9	4	11	6	1	8	3	10	5

Modes:
Display
I - Ionian, II - Dorian, III - Phrygian, IV - Lydian, V - Mixolydian, VI - Aeolian, VII - Locrian

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

Roles:
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

```