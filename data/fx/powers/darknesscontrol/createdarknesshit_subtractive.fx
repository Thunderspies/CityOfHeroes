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
		
		Part	:SubtractiveFlash1.part
		Part	:CreateDarknessPuddle_Subtractive_Local.part
		Part1	:CreateDarknessPuddle_Subtractive.part
		Part2	:CreateDarknessPuddle2_Subtractive.part
	End

	Event	

		ENAME	BaseTrail
		Type	Posit
		At	root

		Part1	:CreateDarknessTrail_Subtractive.part
		Part2	:CreateDarknessTrail2_Subtractive.part
		

	End

	Event
		EName	darkskirt
		Type	Posit #Start
		At	ROOT
		
		Part1	:CreateDarknessPuddleFlat_Subtractive.part
		Part2	:CreateDarknessPuddle2Flat_Subtractive.part
	End
End

End			