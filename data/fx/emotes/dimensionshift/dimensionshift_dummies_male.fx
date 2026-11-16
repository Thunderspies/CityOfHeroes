#############################################################
## DimensionShift_Dummies.fx
#############################################################

FxInfo
LifeSpan 200

#############################################################
Condition
	On	Time
	Time	0

	Event
		EName	DummyAnchor
		Type	Local
		At	Root
		Anim	FX_DimensionShift
		Lifespan 105
	End
End

Condition
	On 	Time
	Time 	4

	Event
		Type	Local
		At	DummyAnchor
		AnimPiv UArmL
		Bhvr	WEAPONS\DualPistols\FX_DualPistols_MBlur1.bhvr
		BhvrOverride
			StartColor		127 100 25
		End
		Anim	FX_MotionBlur_Male
		Lifespan 60
	End

	Event
		Type	Local
		At	DummyAnchor
		AnimPiv UArmR
		Bhvr	WEAPONS\DualPistols\FX_DualPistols_MBlur1.bhvr
		BhvrOverride
			StartColor		127 100 25
		End
		Anim	FX_MotionBlur_Male
		Lifespan 60
	End
End

Condition
	On 	Time
	Time 	8

	Event
		Type	Local
		At	DummyAnchor
		AnimPiv LArmL
		Bhvr	WEAPONS\DualPistols\FX_DualPistols_MBlur1.bhvr
		BhvrOverride
			StartColor		83 50 25
		End
		Anim	FX_MotionBlur_Male
		Lifespan 60
	End

	Event
		Type	Local
		At	DummyAnchor
		AnimPiv LArmR
		Bhvr	WEAPONS\DualPistols\FX_DualPistols_MBlur1.bhvr
		BhvrOverride
			StartColor		83 50 25
		End
		Anim	FX_MotionBlur_Male
		Lifespan 60
	End
End

Condition
	On 	Time
	Time 	12

	Event
		Type	Local
		At	DummyAnchor
		AnimPiv HandL
		Bhvr	WEAPONS\DualPistols\FX_DualPistols_MBlur1.bhvr
		BhvrOverride
			StartColor		50 25 65
		End
		Anim	FX_MotionBlur_Male
		Lifespan 60
	End

	Event
		Type	Local
		At	DummyAnchor
		AnimPiv HandR
		Bhvr	WEAPONS\DualPistols\FX_DualPistols_MBlur1.bhvr
		BhvrOverride
			StartColor		50 25 65
		End
		Anim	FX_MotionBlur_Male
		Lifespan 60
	End
End

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	DummyAnchor
		AnimPiv UArmL
		BhvrOverride
			PositionOffset	0.0 3.5 0.0
		End
		Part	:Burst_Star_Small.part
	End

	Event
		Type	Local
		At	DummyAnchor
		AnimPiv UArmR
		BhvrOverride
			PositionOffset	0.0 3.5 0.0
		End
		Part	:Burst_Star_Small.part
	End
End

Condition
	On 	Time
	Time 	9

	Event
		Type	Local
		At	DummyAnchor
		AnimPiv LArmL
		BhvrOverride
			PositionOffset	0.0 3.5 0.0
		End
		Part	:Burst_Star_Small.part
	End

	Event
		Type	Local
		At	DummyAnchor
		AnimPiv LArmR
		BhvrOverride
			PositionOffset	0.0 3.5 0.0
		End
		Part	:Burst_Star_Small.part
	End
End

Condition
	On 	Time
	Time 	13

	Event
		Type	Local
		At	DummyAnchor
		AnimPiv HandL
		BhvrOverride
			PositionOffset	0.0 3.5 0.0
		End
		Part	:Burst_Star_Small.part
	End

	Event
		Type	Local
		At	DummyAnchor
		AnimPiv HandR
		BhvrOverride
			PositionOffset	0.0 3.5 0.0
		End
		Part	:Burst_Star_Small.part
	End
End
#############################################################

End