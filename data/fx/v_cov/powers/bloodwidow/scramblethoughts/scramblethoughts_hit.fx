#############################################################
## Blood Widow "Scramble Thoughts" Hit FX
#############################################################

FxInfo

LifeSpan 50

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	faceringsRigging
		Type	local
		At	Head
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	POWERS\Psionics\InfuseLight2.part
		Part2	POWERS\Psionics\Infuseglow.part
		Part3	POWERS\Psionics\InfuseRings.part
		Part4	POWERS\Psionics\PercentSign.part
		Part5	POWERS\Psionics\Asterick.part
		Sound Infuse4 80 80 .6
	End

	Event
		EName	facerings
		Type	local
		At	Head
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	POWERS\Psionics\InfuseLight2.part
		Part2	POWERS\Psionics\Infuseglow.part
		Part3	POWERS\Psionics\Defuseglow.part
		Part4	POWERS\Psionics\QuestionMark.part
		Part5	POWERS\Psionics\NumberSign.part
	End

	Event
		EName	facerings2
		Type	local
		At	Head
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	POWERS\Psionics\PsionicBubblesHit.part
		Part2	POWERS\Psionics\HitRaysBig.part
		Part4	POWERS\Psionics\HitStreaksBig.part
		Lifespan 25
	End
End

#############################################################

End