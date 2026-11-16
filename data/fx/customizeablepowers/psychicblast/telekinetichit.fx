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
		Part1	CustomizeablePowers\PsychicBlast\InfuseLight2.part
		Part2	CustomizeablePowers\PsychicBlast\Infuseglow.part
		Part3	CustomizeablePowers\PsychicBlast\InfuseRings.part
		Sound Infuse4 80 80 .6
	End

	Event
		Type	local
		At	Head
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part4	CustomizeablePowers\PsychicBlast\PercentSign.part
		Part5	CustomizeablePowers\PsychicBlast\Asterick.part
		Part4	CustomizeablePowers\PsychicBlast\QuestionMark.part
		Part5	CustomizeablePowers\PsychicBlast\NumberSign.part
	End

	Event
		Type	local
		At	Head
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\PsychicBlast\PsionicBubblesHit.part
		Part2	CustomizeablePowers\PsychicBlast\HitRaysBig.part
		Part4	CustomizeablePowers\PsychicBlast\HitStreaksBig.part
		Lifespan 25
	End

	Event
		Type	local
		At	Head
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\PsychicBlast\TelekineticRingBig.part
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
		Part1	CustomizeablePowers\PsychicBlast\TelekineticRingBig.part
		Lifespan 10
	End
End

#############################################################

End