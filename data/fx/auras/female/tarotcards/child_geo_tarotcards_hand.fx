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
			Spin		0.0 0.1 0.0
			SpinJitter	0.005 0.03 0.005
		End
	End
End

Condition
	On	Time
	Time	0
	Random	1

	Event
		Ename	GEO_Leaves
		Type	Local
		At	SpinnerOffset
		Bhvr	behaviors/Scale_Cards.bhvr

		BhvrOverride
			PositionOffset		0.5 0.5 0.0
			StartJitter		0.15 0.15 0.15
			SpinJitter		.1 .1 .1
			InitialVelocityJitter	0.01 0.01 0.01
			Gravity			0.0002
		End
		Anim	FX_TarotCards_TheLovers
		Lifespan 40
	End

	Event
		Ename	GEO_Leaves2
		Type	Local
		At	SpinnerOffset
		Bhvr	behaviors/Scale_Cards.bhvr

		BhvrOverride
			PositionOffset		0.5 0.5 0.0
			StartJitter		0.15 0.15 0.15
			SpinJitter		.1 .1 .1
			InitialVelocityJitter	0.01 0.01 0.01
			Gravity			0.0002
		End
		Anim	FX_TarotCards_TheTower
		Lifespan 40
	End

	Event
		Ename	GEO_Leaves3
		Type	Local
		At	SpinnerOffset
		Bhvr	behaviors/Scale_Cards.bhvr

		BhvrOverride
			PositionOffset		0.5 0.5 0.0
			StartJitter		0.15 0.15 0.15
			SpinJitter		.1 .1 .1
			InitialVelocityJitter	0.01 0.01 0.01
			Gravity			0.0002
		End
		Anim	FX_TarotCards_TheMoon
		Lifespan 40
	End

	Event
		Ename	GEO_Leaves4
		Type	Local
		At	SpinnerOffset
		Bhvr	behaviors/Scale_Cards.bhvr

		BhvrOverride
			PositionOffset		0.5 0.5 0.0
			StartJitter		0.15 0.15 0.15
			SpinJitter		.1 .1 .1
			InitialVelocityJitter	0.01 0.01 0.01
			Gravity			0.0002
		End
		Anim	FX_TarotCards_TheSun
		Lifespan 40
	End

	Event
		Ename	GEO_Leaves5
		Type	Local
		At	SpinnerOffset
		Bhvr	behaviors/Scale_Cards.bhvr

		BhvrOverride
			PositionOffset		0.5 0.5 0.0
			StartJitter		0.15 0.15 0.15
			SpinJitter		.1 .1 .1
			InitialVelocityJitter	0.01 0.01 0.01
			Gravity			0.0002
		End
		Anim	FX_TarotCards_Death
		Lifespan 40
	End
End

#############################################################

End