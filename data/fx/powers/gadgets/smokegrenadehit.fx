#########################################################
##	Fireweapon
########################################################
FxInfo

Input
	InpName	Root
End

LifeSpan	70

######################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	StoneDustRingHit
		Type	start
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:TearGas01.part
		Part1	:TearGasFlat.part
		Sound SmokeGrExplo 80 80 .8
		LifeSpan	30
	End

End


End
##################################

