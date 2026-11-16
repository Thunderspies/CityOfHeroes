#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition

	On 	Time
	Time 	0

	Event
		EName	WingSkeleton
		Type	Local
		At	Chest
		Bhvr	behaviors/WingScale_Angel_Male.bhvr
		Anim	FX_wings_Celestial
		BhvrOverride
			Alpha			1
			Scale			0.55 0.55 0.55
			PositionOffset		0.0 0.0 -1.0
			StartColor		0 0 255
		End
	End
End

Condition
	On		Time
	Time		0
	DoMany		1

	Event
		EName	WingTrailL
		Type	Local
		At	WingSkeleton
		AnimPiv	ToeL
		Flags	OneAtATime
		Part	:Celestial_Tendril_Trail.part

	End

	Event
		EName	WingTrailR
		Type	Local
		At	WingSkeleton
		AnimPiv	ToeR
		Flags	OneAtATime
		Part	:Celestial_Tendril_Trail.part
	End

	Event
		EName	WingTrailCentral
		Type	Local
		At	WingSkeleton
		Flags	OneAtATime
		Part	:Celestial_Trail.part
	End
End

#############################################################

End