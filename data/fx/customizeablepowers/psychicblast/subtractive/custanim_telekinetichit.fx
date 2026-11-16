#############################################################
## TelekineticHit.fx
#############################################################

FxInfo

Input
	Inpname	Hips
End

LifeSpan 45

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	Head
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	:InfuseLight2.part
		Part2	:Infuseglow.part
		Part3	:InfuseRings.part
		Sound Infuse4 80 80 .6
	End

	Event
		Type	local
		At	Head
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part4	:PercentSign.part
		Part5	:Asterick.part
		Part4	:QuestionMark.part
		Part5	:NumberSign.part
	End

	Event
		Type	local
		At	Head
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	:PsionicBubblesHit.part
		Part2	:HitRaysBig.part
		Part4	:HitStreaksBig.part
		Lifespan 25
	End

	Event
		Type	local
		At	Head
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	:TelekineticRingBig.part
		Lifespan 10
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	local
		At	Head
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	:TelekineticRingBig.part
		Lifespan 10
	End
End

#############################################################

End