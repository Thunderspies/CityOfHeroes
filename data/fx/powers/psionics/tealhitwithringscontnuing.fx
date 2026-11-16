#############################################################
## TealHitWithRingsContnuing.fx
#############################################################

FxInfo

Input
	InpName	head
End

Input
	InpName	root
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound PsionicsTealHit_loop 60 60 .36
		bhvr	behaviors/soundFade3.bhvr
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

#		part1	:TealHitMist.part
		part2	:PurpleInfuseRings.part
		Part4	:QuestionMark.part
		Part5	:NumberSign.part
		Part3	:PercentSign.part

	End

	Event
		EName	facerings1
		Type	local
		At	Head

		Part1	:PsionicBubblesHit.part
		Part2	:HitRaysBig2.part
		Part4	:HitStreaksBig2.part
		Part5	:Asterick.part
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
		Part1	:PurpleInfuseRingsDown.part

	End

End

End


