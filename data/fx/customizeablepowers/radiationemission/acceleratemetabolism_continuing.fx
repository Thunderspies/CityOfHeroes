#############################################################
## AccelerateMetabolism_Continuing.FX
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	7

	Event
		Ename	LightRays
		Type	Posit
		At	Chest
		Part2	CustomizeablePowers\RadiationEmission\HitRing.part
	End
End

Condition
	On	Time
	Time	13

	Event
		Ename	LightRays
		Type	Posit
		At	Chest
		Part2	CustomizeablePowers\RadiationEmission\HitRing2.part
	End
End

Condition
	On	Time
	Time	0

	Event
		Ename	LightRays
		Type	Posit
		At	Chest
		part1	CustomizeablePowers\RadiationEmission\RadiationLightHit.part
		Part2	CustomizeablePowers\RadiationEmission\BurstRingsContinuing.Part
	End
End

#############################################################

End