#############################################################
## Radiant Aura
#############################################################

FxInfo

LifeSpan 120

#############################################################

Condition
	On	Time
	Time	0
	Event
		EName	Rings
		Type	local
		At	Root
		Part4	POWERS\RadiationControl\RadiationRings.part
		Part5	POWERS\RadiationControl\RadiationRingBurst.part
		Sound darkregen 66 66 0.5
	End
End

Condition
	On	Time
	Time	7

	Event
		EName	Burst
		Type	local
		At	Root
		Part5	POWERS\RadiationControl\RadiationRingBurst.part
		Sound radcharge 66 66 0.55
	End
End

Condition
	On	Time
	Time	15
	Event
		EName	Burst
		Type	local
		At	Root
		Part1	POWERS\RadiationControl\RadiationLightHitLocal.part
	End
End

Condition
	On	Time
	Time	21
	Event
		EName	Burst2
		Type	local
		At	Root
		Sound radiation5_loop 66 66 0.75
	End
End

Condition
	On	Time
	Time	30

	Event
		EName	RingsContinued
		Type	local
		At	Root
		Part4	POWERS\RadiationControl\RadiationRingsContinued.part
	End

	Event
		EName	Rings
		Type	local
		At	Root
		Part4	POWERS\RadiationControl\RadiationRingsFlat.part
	End
End

#############################################################

Condition
	On	Time
	Time	38

	Event
		EName	HitSound
		Type	local
		At	Root
		Sound heal1 70 70 0.8
	End
End

#############################################################

End