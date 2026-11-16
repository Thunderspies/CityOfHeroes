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
		At	Hips
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 		0 .1 .5
			PyrRotateJitter		0 0 -90
			Spin			0.003 0.003 0.03
		End
		ChildFX	:Seed_Indicator_Purple_Child.fx
	End
End

#############################################################
End