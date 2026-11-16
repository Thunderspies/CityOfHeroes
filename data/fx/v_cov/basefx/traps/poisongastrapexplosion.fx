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
		BhvrOverride
			PositionOffset -1 .2 0
		End
		Part	:TearGas01.part
		Part	:PoisonYellow2.part
		Part	:PoisonGlows.part
		Part	:MustardGasSpecks.part
		LifeSpan	30
	End

End


End
##################################

