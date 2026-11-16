#############################################################
## CHILD_GEO_Leaves.fx
#############################################################

FxInfo

#########################################################
Condition
	On	Time
	Time	0

	Event
		EName	SpinnerOffset
		Type	StartPositOnly
		At	Origin
		BhvrOverride
			Spin		0.0 0.015 0.0
			SpinJitter	0.005 0.005 0.005
		End
	End

	Event
		Ename	GEO_Leaves
		Type	Local
		At	SpinnerOffset
		Bhvr	behaviors/Scale_Cards.bhvr
		BhvrOverride
			Scale			0.1 0.1 0.1
			ScaleRate		1.05 1.05 1.05
			EndScale		0.25 0.25 0.25
			PositionOffset		0.0 0.2 0.0
			StartJitter		0.0 0.0 0.0
			SpinJitter		.1 .1 .1
			InitialVelocityJitter	0.01 0.01 0.01
			Gravity			0.00035
		End
		Lifespan 60
		Anim	FX_Leaves
	End
End

#############################################################

End