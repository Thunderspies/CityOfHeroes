#############################################################
## Radiant Aura
#############################################################

FxInfo

LifeSpan	120

#############################################################

Condition
	On	Time
	Time	0
	Event
		EName	Rings
		Type	local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part4	CustomizeablePowers\Empathy\RadiationRings.part
		Part5	CustomizeablePowers\Empathy\RadiationRingBurst.part
		Sound darkregen 66 66 .5
	End
End

Condition
	On	Time
	Time	7

	Event
		EName	Burst
		Type	local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part5	CustomizeablePowers\Empathy\RadiationRingBurst.part
		Sound radcharge 66 66 .55
	End
End

Condition
	On	Time
	Time	15
	Event
		EName	Burst
		Type	local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\Empathy\RadiationLightHitLocal.part
	End
End

Condition
	On	Time
	Time	21
	Event
		EName	Burst2
		Type	local
		At	Hips
		Sound radiation5_loop 66 66 .75
	End
End

Condition
	On	Time
	Time	30

	Event
		EName	RingsContinued
		Type	local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part4	CustomizeablePowers\Empathy\RadiationRingsContinued.part
	End

	Event
		EName	Rings
		Type	local
		At	Root
		Bhvr	Behaviors\GenericParticleFade.bhvr
		#Part3	:RadiationRingsContinuedFlat.part
		Part4	CustomizeablePowers\Empathy\RadiationRingsFlat.part
	End
End

#############################################################

Condition
	On	Time
	Time	38

	Event
		EName	HitSound
		Type	local
		At	Hips
		Sound heal1 70 70 .8
	End
End

#############################################################

End