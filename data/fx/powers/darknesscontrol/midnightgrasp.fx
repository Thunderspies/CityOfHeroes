#########################################################
##	chill touch hit
FxInfo


LifeSpan	70

##################################################################################

Condition
	On	Time
	Time	10
	

	Event
		EName	12
		Type	start
		At	WepL
		Bhvr	Behaviors/projectilemidnightGrasp.bhvr
		Part	:MidnightHandTendril.part
		Sound MidnightGrasp 70 70 .85
		Magnet	T_Root
		Lookat	T_Root
	End

	
	
End


End				