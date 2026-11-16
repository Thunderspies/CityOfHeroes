#############################################################
## Blood Widow "Psychic Wail" FX
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
		Geom	LocalPivot01
		Sound mentalwail2 80 80 .75
	End

	Event
		EName	core3
		Type	Local
		At	core
		Altpiv	4
		part1	POWERS\Psionics\CryRays.part
		part2	POWERS\Psionics\PsionicGlow2.part
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
		part1	POWERS\Psionics\PsionicRingsSmall.part
		part2	POWERS\Psionics\PsionicRingzSmall.part
		Lifespan 34
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	OrbMist
		Type	Local
		At	root
		Bhvr	behaviors\Rune_spinFastSmall.bhvr
		geom	FX_RuneStreak
		Lifespan 50
	End

	Event
		EName 	OrbStreak
		Type	Local
		At	root
		Bhvr	behaviors\Psionic_Spin.bhvr
		geom	FX_RuneStreak
	End

	Event
		EName 	Orb
		Type	Local
		At	OrbStreak
		Altpiv	1
		Part1	POWERS\Psionics\GlowOrbStar2Warm.part
		part3	POWERS\Psionics\GlowOrbTailWarm2.part
		Lifespan 63
	End
End

Condition
	On 	Time
	Time 	35
	Event
		EName 	AOERings
		Type	Local
		At	root
		part1	POWERS\Psionics\PsionicCryRing.Part
		part2	POWERS\Psionics\PsionicCryRing2.Part
		part3	POWERS\Psionics\GlowRings.part
		Lifespan 25
	End

	Event
		EName 	AOERings2
		Type	Local
		At	root
		part4	POWERS\Psionics\DeepScanGlow.part
		Lifespan 28
	End
End

#############################################################

End