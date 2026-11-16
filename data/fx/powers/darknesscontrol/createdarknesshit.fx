#########################################################
##	
##
FxInfo

Input  
	InpName	ROOT
End

LifeSpan	200
##########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	darkskirt
		Type	Posit #Start
		At	ROOT
		
		Part1	:CreateDarknessPuddle.part
		Part2	:CreateDarknessPuddle2.part
	End

	Event	

		ENAME	BaseTrail
		Type	Posit
		At	root

		Part1	:CreateDarknessTrail.part
		Part2	:CreateDarknessTrail2.part
		

	End

	Event
		EName	darkskirt
		Type	Posit #Start
		At	ROOT
		
		Part1	:CreateDarknessPuddleFlat.part
		Part2	:CreateDarknessPuddle2Flat.part
	End
End

End			