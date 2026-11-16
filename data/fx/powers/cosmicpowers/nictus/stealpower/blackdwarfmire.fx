#########################################################
## Black Dwarf Mire
#########################################################

FxInfo

LifeSpan	70

## SOUND FX ######################################################

Condition
	On	Time
	Time	0
	
	Event
		EName	x
		Type	start
		At	Root
		Sound WarSteal 80 80 .86
	End
End

## GROUND FX ######################################################

Condition
	On	Time
	Time	0
	
	Event
		EName	x
		Type	start
		At	Root
		part1	:TendrilRing.part
		part2	:TendrilRingFlat.part
		part1	:TendrilRingBlue.part
		part2	:TendrilRingFlatBlue.part
		part1	:TendrilRingFlatBurstPurple.part
		part2	:TendrilRingFlatBurst2Purple.part
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
		part1	:TendrilRingFlatBurst.part
		part2	:TendrilRingFlatBurst2.part
		part1	:TendrilRingFlatBurstBlue.part
		part2	:TendrilRingFlatBurst2Blue.part		
		part1	:TendrilRingFlatBurstPurple.part
		part2	:TendrilRingFlatBurst2Purple.part
		LifeSpan	3
	End
End

Condition
	On	Time
	Time	36

	Event
		EName	y
		Type	start
		At	Root
		Part3	:BlacktoYellowRing.part
		Part4	:BlacktoYellowRing2.part
		Part5	:BlacktoYellowRing3.part
		Part1	:BlacktoYellowRing4.part
		LifeSpan	17
	End
End

Condition
	On	Time
	Time	56

	Event
		Type	start
		At	Chest
		part	:HighLightStreaksPurple.part
		part	:HighLightStreaks.part
		part	:HighLightStreaksBlue.part
	End
End

########################################################

End				