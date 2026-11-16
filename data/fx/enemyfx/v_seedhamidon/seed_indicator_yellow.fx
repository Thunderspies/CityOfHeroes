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
		At	chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0 0 1
			PyrRotate		0 0 0
			Spin			0.002 0.001 -0.02
		End
		ChildFX	:Seed_Indicator_Yellow_Child.fx
	End
End

#############################################################
End