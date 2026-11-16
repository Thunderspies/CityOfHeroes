#########################################################
##	FireRing
##
FxInfo

LifeSpan	100

Input
	InpName	Hips
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	OrbMist
		Type	Local
		At	root
		geom	FX_RuneStreak
		bhvr	behaviors/Rune_spinFastSmall.bhvr
		part1	:GlowRings.part
		part2	:DeepScanGlow.part

	End

	Event
		EName 	OrbStreak
		Type	Local
		At	root
		geom	FX_RuneStreak
		bhvr	behaviors/Psionic_Spin.bhvr

	End

	Event
		EName 	Orb
		Type	Local
		At	OrbStreak
		altpiv  1
		Part1	:GlowOrbStar2Warm.part

		part3	:GlowOrbTailWarm2.part


	End

End

Condition
	On 	Time
	Time 	50

	Event
		EName 	OrbMist
		Type	Destroy

	End

End

Condition
	On 	Time
	Time 	80

	Event
		EName 	Orb
		Type	Destroy

	End

End

End