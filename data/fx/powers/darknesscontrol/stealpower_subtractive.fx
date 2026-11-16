#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	Root
End

Input  
	InpName	CHEST
End

LifeSpan	70

##################################################################################

Condition
	On	Time
	Time	0
	
	Event
		EName	x
		Type	start
		At	Root
		part1	:TendrilRing.part
		part2	:TendrilRingFlat.part
		part1	:TendrilRing_Subtractive.part
		part2	:TendrilRingFlat_Subtractive.part
		Sound Darkmoonbeam3 80 80 .8

		LifeSpan	46
	End
	
End

Condition
	On	Time
	Time	43
	
	Event
		EName	x
		Type	start
		At	Root
		part1	:TendrilRingFlatBurst_Subtractive.part
		part2	:TendrilRingFlatBurst2_Subtractive.part
		

		LifeSpan	3
	End
	
End

Condition
	On	Time
	Time	45

	Event
		EName	y
		Type	start
		At	Root
		
		Splat	Generic_Ring.tga
		Bhvr	:BlackYellowRingIn.bhvr

		LifeSpan	10
	End

End

Condition
	On	Time
	Time	49

	Event
		EName	y
		Type	start
		At	Root
		
		Splat	ThinRing.tga
		Bhvr	:BlackYellowRingIn2.bhvr

		LifeSpan	10
	End

End

Condition
	On	Time
	Time	53

	Event
		EName	y
		Type	start
		At	Root
		
		Splat	Generic_Ring.tga
		Bhvr	:BlackYellowRingIn3.bhvr

		LifeSpan	10
	End

End

Condition
	On	Time
	Time	55

	Event
		EName	y
		Type	start
		At	Root
		
		Splat	ThinRing.tga
		Bhvr	:BlackYellowRingIn4.bhvr

		LifeSpan	10
	End
End

Condition

#	Event
#		EName	y
#		Type	start
#		At	Root
#		Part3	:BlacktoYellowRing.part
#		Part4	:BlacktoYellowRing2.part
#		Part5	:BlacktoYellowRing3.part
#		Part1	:BlacktoYellowRing4.part
#		LifeSpan	17
#	End

	Event
		EName	15
		Type	start
		At	Chest
		part1	Powers/DarknessControl/HighLightRays.part
		part2	Powers/DarknessControl/HighLightStreaks.part
		part3	Powers/DarknessControl/HighLightRaysMulticolor.part
		part4	Powers/DarknessControl/HighLightStreaksMulticolor.part
	End

End


End				