#############################################################
## SmokeGrenadeHit.fx
#############################################################

FxInfo
LifeSpan	70

#############################################################

Input
	Inpname	Hips
End

######################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	StoneDustRingHit
		Type	start
		At	Root
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Gadgets\TearGas01.part
		Part1	CustomizeablePowers\Gadgets\TearGasFlat.part
		Sound SmokeGrExplo 80 80 .8
		LifeSpan	30
	End

End


End
##################################

