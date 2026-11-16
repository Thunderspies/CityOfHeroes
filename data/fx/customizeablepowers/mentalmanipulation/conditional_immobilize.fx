#############################################################
## Conditional_Immobilize.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound PsionicsTealHit_loop 60 60 .36
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 420
	End
End



Condition
	On 	Time
	Time 	0

	Event

		EName 	Hit
		Type	local
		At	head

		part2	CustomizeablePowers\MentalManipulation\PurpleInfuseRings.part
		Part4	CustomizeablePowers\MentalManipulation\QuestionMark.part
		Part5	CustomizeablePowers\MentalManipulation\NumberSign.part
		Part3	CustomizeablePowers\MentalManipulation\PercentSign.part

	End

	Event
		EName	facerings1
		Type	local
		At	Head

		Part1	CustomizeablePowers\MentalManipulation\PsionicBubblesHit.part
		Part2	CustomizeablePowers\MentalManipulation\HitRaysBig2.part
		Part4	CustomizeablePowers\MentalManipulation\HitStreaksBig2.part
		Part5	CustomizeablePowers\MentalManipulation\Asterick.part
	End

	Event
		EName	facerings2
		Type	local
		At	root

		geom	RootToHeadAdjustment

	End

	Event
		EName	facerings3
		Type	local
		At	facerings2
		altpiv	1
		Part1	CustomizeablePowers\MentalManipulation\PurpleInfuseRingsDown.part

	End

End

End


