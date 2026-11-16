#########################################################
##	chill touch hit
FxInfo

############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	darkskirt
		Type	posit #Start
		At	ROOT
		Part	:RotatingCirclesDark.part
		Part	:RotatingCircles.part
		Part	:DarkServentRings.part
		Part	:FlatCircles.part
		Part	:FlatCirclesGlow.part

	End

	Event
		EName	darkskirt
		Type	posit #Start
		At	ROOT
		
		Part1	:CreateDarknessPuddle.part
		Part2	:CreateDarknessPuddle2.part
	End

	Event	

		ENAME	BaseTrail
		Type	posit
		At	root

		Part1	:CreateDarknessTrail.part
		Part2	:CreateDarknessTrail2.part
		

	End

	Event
		EName	darkskirt
		Type	posit #Start
		At	ROOT
		
		Part1	:CreateDarknessPuddleFlat.part
		Part2	:CreateDarknessPuddle2Flat.part
	End

End

End			