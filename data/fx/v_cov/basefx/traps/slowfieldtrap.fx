#########################################################
##	Fireweapon
########################################################
FxInfo

######################################################################

Condition
	On 	Cycle
	Time 	60

	Event
		EName 	StoneDustRingHit4
		Type	start
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 0 0
		End
		Part	:SlowHaze01.part
		LifeSpan	30
	End

End

End
##################################

