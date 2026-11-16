#########################################################
##	
##
FxInfo

#LifeSpan	200
##########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	darkskirt
		Type	Posit #Start
		At	ROOT
		
		Part	:SubtractiveFlash.part
		Part	:2CreateDarknessPuddle_Subtractive_Local.part
		Part1	:2CreateDarknessPuddle_Subtractive.part
		Part2	:2CreateDarknessPuddle2_Subtractive.part

	End

	Event	

		ENAME	BaseTrail
		Type	Posit
		At	root

		Part1	:2CreateDarknessTrail_Subtractive.part
		Part2	:2CreateDarknessTrail2_Subtractive.part
		

	End

	Event
		EName	darkskirt
		Type	Posit #Start
		At	ROOT
		
		Part1	:2CreateDarknessPuddleFlat_Subtractive.part
		Part2	:2CreateDarknessPuddle2Flat_Subtractive.part
	End
End

End			