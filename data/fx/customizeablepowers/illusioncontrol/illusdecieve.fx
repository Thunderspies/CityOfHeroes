#############################################################
## IllusDecieve.fx
#############################################################

FxInfo

Lifespan 150

#############################################################

Condition
	On	Time
	Time 	8

	Event
		Type	Local
		At 	Origin
		Sound deceive1 100 100 0.7
	End
End

#############################################################

Condition
	On	Time
	Time	10

	Event
		ENAME	LeftHand
		Type	Posit
		At	WepL
		part	CustomizeablePowers\IllusionControl\IllusCastGlow01.part
		Lifespan 108
	End

	Event
		ENAME	RightHand
		Type	Posit
		At	WepR
		part	CustomizeablePowers\IllusionControl\IllusCastGlow01.part
		Lifespan 108
	End
End

#############################################################

Condition
	On	Time
	Time	23

	Event
		ENAME	LeftHand
		Type	Posit
		At	WepL
		part	CustomizeablePowers\IllusionControl\IllusCastGlow02.part
		Lifespan 21
	End

	Event
		ENAME	RightHand
		Type	Posit
		At	WepR
		part	CustomizeablePowers\IllusionControl\IllusCastGlow02.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	68

	Event
		ENAME	LeftHand
		Type	Posit
		At	WepL
		part	CustomizeablePowers\IllusionControl\IllusCastGlow02.part
		Lifespan 20
	End

	Event
		ENAME	RightHand
		Type	Posit
		At	WepR
		part	CustomizeablePowers\IllusionControl\IllusCastGlow02.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	108

	Event
		ENAME	LeftHand
		Type	Posit
		At	WepL
		part	CustomizeablePowers\IllusionControl\IllusCastGlow02.part
		Lifespan 20
	End

	Event
		ENAME	RightHand
		Type	Posit
		At	WepR
		part	CustomizeablePowers\IllusionControl\IllusCastGlow02.part
		Lifespan 20
	End
End

#############################################################

End