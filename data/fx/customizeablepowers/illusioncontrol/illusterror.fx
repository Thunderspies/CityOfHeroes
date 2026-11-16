#########################################################
##	template

FxInfo
Lifespan 150

#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On	Time
	Time 	11

	Event
		Type	Local
		At 	Origin
		Sound deceive1 100 100 .74
	End
End


Condition
	On	Time
	Time 	44

	Event
		Type	Local
		At 	Origin
		Sound terror1 100 100 .66
	End
End



#########################################################
###################  Hand Casts  ########################
#########################################################


Condition
	On	Time
	Time	12

	Event
		ENAME	LeftHand
		Type	Posit
		At	WepL
		part	CustomizeablePowers\IllusionControl\IllusCastGlow01.part
		Lifespan 60

	End

	Event
		ENAME	RightHand
		Type	Posit
		At	WepR
		part	CustomizeablePowers\IllusionControl\IllusCastGlow01.part
		Lifespan 60

	End

End


Condition
	On	Time
	Time	45

	Event
		ENAME	LeftHand
		Type	Posit
		At	WepL
		part	CustomizeablePowers\IllusionControl\IllusCastGlow02.part
		Lifespan 40

	End

	Event
		ENAME	RightHand
		Type	Posit
		At	WepR
		part	CustomizeablePowers\IllusionControl\IllusCastGlow02.part
		Lifespan 41

	End

End

#########################################################
###################    Target    ########################
#########################################################

Condition
	On	Time
	Time	20

	Event
		ENAME	OverHead
		Type	Start
		At	Target
		Geom	RootToChestAdjustment
		LifeSpan 100

	End
End

End