#############################################################
## Blood Widow "Psychic Wail" Hit FX
#############################################################

FxInfo

LifeSpan 100

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	OrbMist
		Type	Local
		At	root
		geom	FX_RuneStreak
		bhvr	behaviors\Rune_spinFastSmall.bhvr
		part1	POWERS\Psionics\GlowRings.part
		part2	POWERS\Psionics\DeepScanGlow.part
		Lifespan 50
	End

	Event
		EName 	OrbStreak
		Type	Local
		At	root
		geom	FX_RuneStreak
		bhvr	behaviors\Psionic_Spin.bhvr
	End

	Event
		EName 	Orb
		Type	Local
		At	OrbStreak
		altpiv  1
		Part1	POWERS\Psionics\GlowOrbStar2Warm.part
		part3	POWERS\Psionics\GlowOrbTailWarm2.part
		Lifespan 80
	End
End

#############################################################

End