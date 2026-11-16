#############################################################
## InfuseThoughtsHit2.fx
#############################################################

FxInfo

Input
	Inpname	Hips
End

LifeSpan 30

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	faceringsRigging
		Type	local
		At	Head
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\PsychicBlast\InfuseLight2.part
		Part2	CustomizeablePowers\PsychicBlast\Infuseglow.part
		Part3	CustomizeablePowers\PsychicBlast\InfuseRings.part
		Part4	CustomizeablePowers\PsychicBlast\PercentSign.part
		Part5	CustomizeablePowers\PsychicBlast\Asterick.part
	End

	Event
		EName	facerings
		Type	local
		At	Head
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\PsychicBlast\InfuseLight2.part
		Part2	CustomizeablePowers\PsychicBlast\Infuseglow.part
		Part3	CustomizeablePowers\PsychicBlast\Defuseglow.part
		Part4	CustomizeablePowers\PsychicBlast\QuestionMark.part
		Part5	CustomizeablePowers\PsychicBlast\NumberSign.part
	End

	Event
		EName	facerings2
		Type	local
		At	Head
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\PsychicBlast\PsionicBubblesHit.part
		Part4	CustomizeablePowers\PsychicBlast\HitStreaksBig.part
		Lifespan 25
	End
End

#############################################################

End