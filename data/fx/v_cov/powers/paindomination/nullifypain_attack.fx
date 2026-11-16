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
		At	Hips
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	:RadiationRings.part
		Sound Nullify_Pain_01 66 66 .75
	End
End

Condition
	On	Time
	Time	7

	Event
		EName	Burst
		Type	local
		At	Hips
	End
End

Condition
	On	Time
	Time	15
	Event
		EName	Burst
		Type	local
		At	Hips
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	:RadiationLightHitLocal.part
	End
End

Condition
	On	Time
	Time	21
	Event
		EName	Burst2
		Type	local
		At	Hips
		Sound radiation5_loop 66 66 .5
	End
End

Condition
	On	Time
	Time	30

	Event
		EName	RingsContinued
		Type	local
		At	Hips
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part4	:RadiationRingsContinued.part
	End

	Event
		EName	Rings
		Type	local
		At	Root
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part4	:RadiationRingsFlat.part
		##Part4	:RadiationRingsFlat2.part
	End
End

#############################################################

End