#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
Lifespan 90
Flags InheritGeoScale

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Sound 	Incarnate_CryonicJudgement_Attack_01 100 100 .8
	End

	Event
		Type	Local
		At	Origin
		ChildFX	IncarnatePowers\Incarnate_Gather.fx
		Lifespan	30
	End
End

Condition
	On	Time
	Time	0
	Repeat	2

	Event
		Type	Local
		At	UArmL
		Bhvr	:Icicles_Small.bhvr
		BhvrOverride
			PositionOffset	-0.35 0.0 0.0
			StartJitter	0.35 0.1 0.1
			TintGeom	1
		End
		Geom	Tintable_Icicle01B
		Geom	Tintable_Icicle02B
		Geom	Tintable_Icicle03B
		Lifespan	90
		LifespanJitter	20
	End

	Event
		Type	Local
		At	UArmR
		Bhvr	:Icicles_Small.bhvr
		BhvrOverride
			PositionOffset	0.35 0.0 0.0
			StartJitter	0.35 0.1 0.1
			TintGeom	1
		End
		Geom	Tintable_Icicle01B
		Geom	Tintable_Icicle02B
		Geom	Tintable_Icicle03B
		Lifespan	90
		LifespanJitter	20
	End
End

Condition
	On	Time
	Time	3
	Repeat	2

	Event
		Type	Local
		At	UArmL
		Bhvr	:Icicles_Small.bhvr
		BhvrOverride

			PositionOffset	-0.35 0.0 0.0
			StartJitter	0.35 0.1 0.1
			TintGeom	1
		End
		Geom	Tintable_Icicle01B
		Geom	Tintable_Icicle02B
		Geom	Tintable_Icicle03B
		Lifespan	90
		LifespanJitter	20
	End

	Event
		Type	Local
		At	UArmR
		Bhvr	:Icicles_Small.bhvr
		BhvrOverride
			PositionOffset	0.35 0.0 0.0
			StartJitter	0.35 0.1 0.1
			TintGeom	1
		End
		Geom	Tintable_Icicle01B
		Geom	Tintable_Icicle02B
		Geom	Tintable_Icicle03B
		Lifespan	90
		LifespanJitter	20
	End
End

Condition
	On	Time
	Time	5
	Repeat	2

	Event
		Type	Local
		At	LArmL
		Bhvr	:Icicles_Small.bhvr
		BhvrOverride
			PositionOffset	-0.35 0.0 0.0
			StartJitter	0.35 0.1 0.1
			TintGeom	1
		End
		Geom	Tintable_Icicle01B
		Geom	Tintable_Icicle02B
		Geom	Tintable_Icicle03B
		Lifespan	90
		LifespanJitter	20
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	:Icicles_Small.bhvr
		BhvrOverride
			PositionOffset	0.35 0.0 0.0
			StartJitter	0.35 0.1 0.1
			TintGeom	1
		End
		Geom	Tintable_Icicle01B
		Geom	Tintable_Icicle02B
		Geom	Tintable_Icicle03B
		Lifespan	90
		LifespanJitter	20
	End
End

Condition
	On	Time
	Time	7
	Repeat	2

	Event
		Type	Local
		At	LArmL
		Bhvr	:Icicles_Small.bhvr
		BhvrOverride
			PositionOffset	-0.35 0.0 0.0
			StartJitter	0.35 0.1 0.1
			TintGeom	1
		End
		Geom	Tintable_Icicle01B
		Geom	Tintable_Icicle02B
		Geom	Tintable_Icicle03B
		Lifespan	90
		LifespanJitter	20
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	:Icicles_Small.bhvr
		BhvrOverride
			PositionOffset	0.35 0.0 0.0
			StartJitter	0.35 0.1 0.1
			TintGeom	1
		End
		Geom	Tintable_Icicle01B
		Geom	Tintable_Icicle02B
		Geom	Tintable_Icicle03B
		Lifespan	90
		LifespanJitter	20

	End
End

Condition
	On	Time
	Time	10
	Repeat	3

	Event
		Type	Local
		At	WepL
		Bhvr	:Icicles.bhvr
		BhvrOverride
			PyrRotate	0 90 0
			PyrRotateJitter	0 0 0
			TintGeom	1
		End
		Geom	Tintable_Icicle01B
		Geom	Tintable_Icicle02B
		Geom	Tintable_Icicle03B
		Lifespan	90
		LifespanJitter	20

	End

	Event
		Type	Local
		At	WepR
		Bhvr	:Icicles.bhvr
		BhvrOverride
			PyrRotate	0 90 0
			PyrRotateJitter	0 0 0

			TintGeom	1
		End
		Geom	Tintable_Icicle01B
		Geom	Tintable_Icicle02B
		Geom	Tintable_Icicle03B
		Lifespan	90
		LifespanJitter	20

	End

End

Condition
	On	Time
	Time	2

	Event
		Type	Local
		At	WepL
		Part	:CryoMist_Big.part
		Part	:CryoMist_Detached.part
		POther	T_Chest
		Lifespan 100
	End

End

Condition
	On	Time
	Time	25

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 5.0 -20.0
		End
		Part	:Back_Snow.part
		Part	:Spray_Snow_Particles_Range.part
		Lifespan 40
	End

	Event
		Type	Start
		At	LArmL
		BhvrOverride
			PYRrotate	0.0 -5.0 -10.0
		End
		Part	:Spray_Snow_Highlights_Range.part
		Lifespan 65
	End
End

Condition
	On	Time
	Time	30

	Event
		EName	MainAnchor
		Type	Local
		At	Root
		Lifespan 45
	End

	Event
		Type	Start
		At	WepL
		Bhvr	behaviors/CameraShake_Subtle60.bhvr
		BhvrOverride
			PYRrotate	0.0 -5.0 4.0
		End
		Part	:Cryonic_VolumeCone_Large.part
		Part	:Cryonic_VolumeCone_Additive_Range.part

		POther	T_Chest
		Lifespan 55
	End

	Event
		Type	Start
		At	WepL
		BhvrOverride
			PYRrotate	0.0 -5.0 -10.0
		End
		Part	:Spray_Snow_Main_Range.part
		POther	T_Chest

		Lifespan 50
	End
End

Condition
	On	Cycle
	Time	4
	Repeat	2
	Event
		Type	Local
		At	MainAnchor
		ChildFX	:CHILD_Judgment_Cryonic.fx
		Lifespan 30

	End
End

#############################################################

End