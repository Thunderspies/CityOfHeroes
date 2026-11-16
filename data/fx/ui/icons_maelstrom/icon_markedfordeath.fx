#############################################################
## Icon_MissionInProgress.fx
#############################################################

FxInfo
LifeSpan 0

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 7.5 -0.5
		End
		Part	:Icon_MarkedForDeath.part
		Part	:Persistent_Glow.part
	End
End

Condition
	On	Cycle
	Time	60

	Event
		Type	Local
		At	Root
		Bhvr	behaviors/GenericParticleFade.bhvr
		Lifespan 	30
		BhvrOverride
			PositionOffset	0 7.5 -0.5
		End
		ChildFX	:Child_Chest.fx
	End
End

#############################################################

End