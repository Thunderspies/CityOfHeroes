#########################################################
##	
##
FxInfo

Input  
	InpName	ROOT
End

LifeSpan	100
##########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	darkskirt
		Type	Local #Start
		At	ROOT
		
		Part1	:CreateDarknessPuddle.part
		Part2	:CreateDarknessPuddle2.part
		Sound grexplo2 80 80 .8
	End

	Event	

		ENAME	BaseTrail
		Type	local
		At	root

		Part1	:CreateDarknessTrail.part
		Part2	:CreateDarknessTrail2.part
		Part3	:CreateDarknessPuddleFlat.part
		Part4	:CreateDarknessPuddle2Flat.part
		Part5	:DarkTarRing.Part
		Sound forcefield5_loop 80 80 .7

	End

	Event
		EName	ringBurst
		Type	Local #Start
		At	ROOT
				
		Part4	:ThePitRing.Part
	End
End

Condition
	On 	Time
	Time 	6

	Event
		EName	ringBurst2
		Type	Local #Start
		At	ROOT
				
		Part4	:ThePitRing.Part
	End

End


End			