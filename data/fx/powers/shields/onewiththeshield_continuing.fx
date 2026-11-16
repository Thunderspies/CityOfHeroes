#############################################################
## OneWithTheShield_Continuing.fx
#############################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0 0.25 0
		End
		Part	:OneWithTheShield_Rays1.part
	End
End

#########################################################

Condition
	On	Cycle
	Time	120

	Event
		Type	Local
		At	Root
		ChildFX	:Child_ChestToLArmPulse.fx
		Lifespan 30
	End
End

#############################################################

End