#############################################################
## FX_Seagull_Ultimate.fx
#############################################################

FxInfo
Flags InheritAlpha

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName 	SpinAnchor
		Type	Start
		At	root
		BhvrOverride
			Spin 		0 0.02 0
			SpinJitter 	0 0.005 0
			PYRRotateJitter	0 45 20
		End
	End

	Event
		EName 	BirdAnchor
		Type	Local
		At	SpinAnchor
		BhvrOverride
			PYRRotate	0 90 0
			PositionOffset 	0 0.0 50
			StartJitter	10 10 40
		End
	End

	Event
		EName 	Birds
		Type	Local
		At	BirdAnchor
		Anim	FX_seagull_ultimate
		Bhvr	behaviors/Scale_Cards.bhvr
		BhvrOverride
			FadeOutLength	100
			Endscale	1.0 1.0 1.0
			PYRRotate	0 0 30
		End
		Lifespan 450
	End

End

#############################################################

End