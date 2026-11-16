#########################################################
##	Fireweapon
########################################################
FxInfo

######################################################################

Condition
	On 	Cycle
	Time 	523

	Event
		EName 	StoneDustRingHit4
		Type	start
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-1 0 0
		End
		Part	:TearGas02.part
		Part	:PoisonYellow1.part
		LifeSpan	15
	End

End

Condition
	On 	Cycle
	Time 	251
	Chance	.25

	Event
		EName 	StoneDustRingHit3
		Type	start
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-1 0 0
		End
		Part	:TearGas02.part
		Part	:PoisonYellow1.part
		LifeSpan	20
	End

End

Condition
	On 	Cycle
	Time 	117
	Chance	.1

	Event
		EName 	StoneDustRingHit2
		Type	start
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-1 0 0
		End
		Part	:TearGas02.part
		Part	:PoisonYellow1.part
		LifeSpan	15
	End

End

Condition
	On 	Cycle
	Time 	53
	Chance	.025

	Event
		EName 	StoneDustRingHit
		Type	start
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-1 0 0
		End
		Part	:TearGas02.part
		Part	:PoisonYellow1.part
		LifeSpan	10
	End

End


End
##################################

