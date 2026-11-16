#############################################################
## PsionicCry.fx
#############################################################

FxInfo

LifeSpan 100

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	core
		Type	Local
		At	head
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Geom	LocalPivot01
		Sound mentalwail2 80 80 .75
	End

	Event
		EName	core3
		Type	Local
		At	core
		Altpiv	4
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:CryRays.part
		part2	:PsionicGlow2.part
		Lifespan 60
	End
End

Condition
	On 	Time
	Time 	10

	Event
		EName	core2
		Type	Local
		At	core
		Altpiv	4
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:PsionicRingsSmall.part
		part2	:PsionicRingzSmall.part
		Lifespan 34
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	OrbStreak
		Type	Local
		At	root
		bhvr	:Psionic_Spin.bhvr
		geom	FX_RuneStreak
	End

	Event
		EName 	Orb
		Type	Local
		At	OrbStreak
		altpiv  1
		Part1	:GlowOrbStar2Warm.part
		part3	:GlowOrbTailWarm2.part
		Lifespan 63
	End
End

#############################################################

Condition
	On 	Time
	Time 	35

	Event
		EName 	AOERings
		Type	Local
		At	root
		part1	:PsionicCryRing.Part
		part2	:PsionicCryRing2.Part
		part3	:GlowRings.part
		Lifespan 25
	End

	Event
		EName 	AOERings2
		Type	Local
		At	root
		part4	:DeepScanGlow.part
		Lifespan 28
	End
End

#############################################################

End