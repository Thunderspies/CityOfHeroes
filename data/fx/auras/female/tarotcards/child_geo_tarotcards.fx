#############################################################
## CHILD_GEO_TarotCards.fx
#############################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0

	Event
		EName	SpinnerOffset
		Type	StartPositOnly
		At	Head
		BhvrOverride
			Spin		0.0 0.09 0.0
			SpinJitter	0.0 0.01 0.0
		End
	End

	Event
		Type	Local
		At	SpinnerOffset
		Bhvr	behaviors/GenericParticleFade.bhvr

		BhvrOverride
			PositionOffset		1.5 0.85 0.0
			StartJitter		0.25 1.75 0.25
			Scale			1.0 1.0 1.0
			InitialVelocity		0.0 -0.0 0.0
			InitialVelocityJitter	0.025 0.05 0.025
			Drag			0.025
			Gravity			0.003
		End
		Part	:Sparkles_Continuous.part
		Part	:ColdMist_Additive.part
		Lifespan 	40
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
			PositionOffset		2.0 -1.0 0.0
			StartJitter		1.25 2.25 1.25
			SpinJitter		.5 .5 .5
			InitialVelocity		0.0 -0.0 0.0
			InitialVelocityJitter	0.025 0.05 0.025
			Drag			0.025
			Gravity			0.003
		End
		Anim	FX_TarotCards_TheLovers
		Lifespan 70

	End

	Event
		Ename	GEO_Leaves2
		Type	Local
		At	SpinnerOffset
		Bhvr	behaviors/Scale_Cards.bhvr

		BhvrOverride
			PositionOffset		2.0 1.0 0.0
			StartJitter		0.25 2.25 0.25
			SpinJitter		.5 .5 .5
			InitialVelocity		0.0 -0.0 0.0
			InitialVelocityJitter	0.025 0.05 0.025
			Drag			0.025
			Gravity			0.003
		End
		Anim	FX_TarotCards_TheTower
		Lifespan 70

	End

	Event
		Ename	GEO_Leaves3
		Type	Local
		At	SpinnerOffset
		Bhvr	behaviors/Scale_Cards.bhvr

		BhvrOverride
			PositionOffset		2.0 1.0 0.0
			StartJitter		0.25 2.25 0.25
			SpinJitter		.5 .5 .5
			InitialVelocity		0.0 -0.0 0.0
			InitialVelocityJitter	0.025 0.05 0.025
			Drag			0.025
			Gravity			0.003
		End
		Anim	FX_TarotCards_TheMoon
		Lifespan 70

	End

	Event
		Ename	GEO_Leaves3
		Type	Local
		At	SpinnerOffset
		Bhvr	behaviors/Scale_Cards.bhvr

		BhvrOverride
			PositionOffset		2.0 1.0 0.0
			StartJitter		0.25 2.25 0.25
			SpinJitter		.5 .5 .5
			InitialVelocity		0.0 -0.0 0.0
			InitialVelocityJitter	0.025 0.05 0.025
			Drag			0.025
			Gravity			0.003
		End
		Anim	FX_TarotCards_TheSun
		Lifespan 70

	End

	Event
		Ename	GEO_Leaves3
		Type	Local
		At	SpinnerOffset
		Bhvr	behaviors/Scale_Cards.bhvr

		BhvrOverride
			PositionOffset		2.0 1.0 0.0
			StartJitter		0.25 2.25 0.25
			SpinJitter		.5 .5 .5
			InitialVelocity		0.0 -0.0 0.0
			InitialVelocityJitter	0.025 0.05 0.025
			Drag			0.025
			Gravity			0.003
		End
		Anim	FX_TarotCards_Death
		Lifespan 70
	End
End

#############################################################

End