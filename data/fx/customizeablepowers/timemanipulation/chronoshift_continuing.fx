#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Cycle
	Time	60

	Event
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Streaks_Explode_Continuing.part
		Part	:Streaks_Explode_Moving.part
		ChildFX	:Child_ChestGlow.fx

		Lifespan 10
	End
End


#############################################################

End