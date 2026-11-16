#############################################################
## Seed_Indicator.fx
#############################################################

FxInfo
#############################################################

Condition
	On 	Time
	Time 	0
	Event
		Ename	Indicator
		Type	Local
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 		0 0 0.25
			PyrRotate		0 0 90
			Spin			0.001 0.005 -0.02
		End
		ChildFX	:Seed_Indicator_Green_Child.fx
	End
End

#############################################################
End