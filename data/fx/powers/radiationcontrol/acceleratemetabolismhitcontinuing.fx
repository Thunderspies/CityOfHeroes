#############################################################
## AccelerateMetabolismHitContinuing.fx
#############################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	TriggerBits
	TriggerBits	NONHUMAN

	Event
		Type	Destroy
		EName	ALL
	End
End

#############################################################

Condition
	On	Time
	Time	1

	Event
		Type	Posit
		At	Chest
		part1	POWERS\RadiationControl\RadiationLightHit.part
		Part2	POWERS\RadiationControl\BurstRingsContinuing.Part
	End
End

Condition
	On	Time
	Time	7

	Event
		Type	Posit
		At	Chest
		Part2	POWERS\RadiationControl\HitRing.part
	End
End

Condition
	On	Time
	Time	13

	Event
		Type	Posit
		At	Chest
		Part2	POWERS\RadiationControl\HitRing2.part
	End
End

#############################################################

End