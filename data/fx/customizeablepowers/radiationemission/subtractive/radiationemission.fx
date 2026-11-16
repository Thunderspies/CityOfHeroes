#############################################################
## RadiationEmission.fx
#############################################################

FxInfo
Lifespan 120

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part4	:RadiationRings.part
		Part5	:RadiationRingBurst.part
		Sound darkregen 66 66 .5
	End
End

Condition
	On	Time
	Time	7

	Event
		Type	local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part5	:RadiationRingBurst.part
		Sound radcharge 66 66 .55
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	:RadiationLightHitLocal.part
	End
End

Condition
	On	Time
	Time	30

	Event
		Type	local
		At	Root
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part4	:RadiationRingsFlat.part
	End
End

#############################################################

End