#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	wepR
End

Input  
	InpName	wepL
End

Input  
	InpName	T_Root
End

LifeSpan	70

##################################################################################

Condition
	On	Time
	Time	10
	

	Event
		EName	Prime
		Type	start
		At	WepL
		Bhvr	Behaviors/projectilemidnightGrasp.bhvr
		Part	:SkullTenticlesSubtractive.part
		Part	:SkullTenticlesSubtractive.part
		Part	:SkullTenticlesSubtractive.part
		Part	:SkullTenticles.part
		Part	:SkullTenticles.part
		Part	:SkullTenticles.part
		Part	:Skullsmoke.part
		Part	:Skullsmoke.part
		Sound MidnightGrasp 70 70 .85
		Magnet	T_Root
		Lookat	T_Root
	End

	
	
End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy

	End

End

End				