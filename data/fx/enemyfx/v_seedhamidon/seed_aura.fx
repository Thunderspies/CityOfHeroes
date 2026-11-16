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
		At	Origin
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 		0 0 150
			PyrRotate		0 0 0
			Spin			-0.0002 0.0001 0.002
		End
		ChildFX	:Seed_Indicator_Blue_Child.fx
	End
End

#############################################################
End