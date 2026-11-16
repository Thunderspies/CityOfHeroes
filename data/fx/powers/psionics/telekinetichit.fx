#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	10

Input
	InpName	Hips
End

###########################################################

Condition
	On	Time
	Time	0

	Event
		EName	faceringsRigging
		Type	local
		At	Head
		Part1	:InfuseLight2.part
		Part2	:Infuseglow.part
		Part3	:InfuseRings.part
		Part4	:PercentSign.part
		Part5	:Asterick.part
		Sound Infuse4 80 80 .6
	End

	Event
		EName	facerings
		Type	local
		At	Head

		Part1	:InfuseLight2.part
		Part2	:Infuseglow.part
		Part3	:Defuseglow.part
		Part4	:QuestionMark.part
		Part5	:NumberSign.part
	End

	Event
		EName	facerings2
		Type	local
		At	Head

		Part1	:PsionicBubblesHit.part

		Part3	:TelekineticRingBig.part
		Part4	:HitStreaksBig.part
	End

	Event

		Type	local
		At	Head

		Part2	:HitRaysBig.part
		lifespan	10
	End

End

Condition
	On	Time
	Time	10

	Event
		Type	local
		At	Head

		Part1	:TelekineticRingBig.part
	End

End

Condition
	On Time
	Time 25

	Event
		EName	facerings2
		Type	Destroy
	End

End


End