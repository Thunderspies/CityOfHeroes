#############################################################
## BuildUp_Activation.fx
#############################################################

FxInfo
Lifespan 150

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	Brawling_CombatReadiness_01 200 100 .8
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Start
		At	Root
		Bhvr	:Dust_Expanding.bhvr
		BhvrOverride
			PositionOffset	0.0 0.5 0.0
		End
		Part	:Hit_Smoke_Radial_Big.part
		Part	:Hit_Smoke_Radial_Lingering_Big.part
		Lifespan 90
	End

	Event
		Type 	Start
		At	Root
		BhvrOverride
			PositionOffset	0.0 0.25 0.0
		End
		Part	:Buildup_Shockwave.part
	End

	Event
		Type 	Start
		At	Root
		ChildFX	POWERS\Child_BuildUp_NoSFX.fx
	End
End

#############################################################

End