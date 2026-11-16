#############################################################
## RadiationEmission.fx
#############################################################

FxInfo

Lifespan 120

#############################################################

#Condition
#	On	Time
#	Time	21
#
#	Event
#		Type	local
#		At	Hips
#		Sound	radiation5_loop 66 25 .75
#	End
#End
#
#Condition
#	On	Time
#	Time	38
#
#	Event
#		Type	local
#		At	Hips
#		Sound	heal1 70 25 .8
#	End
#End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part4	CustomizeablePowers\RadiationEmission\RadiationRings.part
		Part5	CustomizeablePowers\RadiationEmission\RadiationRingBurst.part
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
		Part5	CustomizeablePowers\RadiationEmission\RadiationRingBurst.part
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
		Part1	CustomizeablePowers\RadiationEmission\RadiationLightHitLocal.part
	End
End

Condition
	On	Time
	Time	30

	Event
		Type	local
		At	Root
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part4	CustomizeablePowers\RadiationEmission\RadiationRingsFlat.part
	End
End

#############################################################

End