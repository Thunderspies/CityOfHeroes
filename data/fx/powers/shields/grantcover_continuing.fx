#############################################################
## GrantCover_Continuing.fx
#############################################################

FxInfo

Lifespan 150

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		EName	ChestOffset
		Type	Local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0 0.25 0
		End
		Part	:Shields_Activation_CoreGlow1.part
		Part	:Shields_Activation_CoreStar1.part
		pOther	Offset1
		Lifespan 35
	End
End

#############################################################

Condition
	On	Time
	Time	25

	Event
		Ename	BodyAura_Initial
		Type	Local
		At	Origin
		ChildFX	:Shields_Continuing_Pulse.fx
		Lifespan 60
	End
End

## RICHOCETS #######################################################

Condition
	On 	Time
	Time 	40

	Event
		Type	Local
		At	Root
		ChildFX	:Child_Richochets.fx
	End
End

#########################################################

End