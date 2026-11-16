#############################################################
## IllusSpecWounds.fx
#############################################################

FxInfo
Lifespan 100

#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On 	Time
	Time	12

	Event
		Type 	Local
		At 	Origin
		Sound illusiontoss3 100 100 .75
	End
End


#########################################################
###################  Hand Casts  ########################
#########################################################


Condition
	On	Time
	Time	15

	Event
		ENAME	LeftHand
		Type	Posit
		At	WepL
		part	:IllusCastGlowRed01.part
		part	:IllusCastGlowRed02.part
		part	:IllusCastGlowRed03.part
		Lifespan 6

	End
End

#############################################################

End
