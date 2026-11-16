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
			PositionOffset		0.0 -0.5 -0.8
			StartColor		0 0 255
		End
	End

#############################################################

	Event
		EName	HandLAnchor
		Type	Local
		At	WingSkeleton
		AnimPiv	HandL
	End

	Event
		EName	HandRAnchor
		Type	Local
		At	WingSkeleton
		AnimPiv	HandR
	End

	Event
		EName	FootRAnchor
		Type	Local
		At	WingSkeleton
		AnimPiv	FootR
	End

	Event
		EName	FootLAnchor
		Type	Local
		At	WingSkeleton
		AnimPiv	FootL
	End

	Event
		EName	ToeRAnchor
		Type	Local
		At	WingSkeleton
		AnimPiv	ToeR
	End

	Event
		EName	ToeLAnchor
		Type	Local
		At	WingSkeleton
		AnimPiv	ToeL
	End

#############################################################

	Event
		EName	WingJointL
		Type	Local
		At	WingSkeleton
		AnimPiv	RingL
		Part	:Celestial_Tendril_Consistent.part
		Part	:Celestial_Tendril.part
		POther	HandLAnchor
		LookAt	HandLAnchor
	End

	Event
		EName	WingJointR
		Type	Local
		At	WingSkeleton
		AnimPiv	RingR
		Part	:Celestial_Tendril_Consistent.part
		Part	:Celestial_Tendril.part
		POther	HandRAnchor
		LookAt	HandRAnchor
	End

	Event
		EName	WingTopL
		Type	Local
		At	WingSkeleton
		AnimPiv	Fore_FootL
		Part	:Celestial_Tendril.part
		Part	:Celestial_Tendril_Core.part
		POther	WingJointL
		LookAt	WingJointL
	End

	Event
		EName	WingTopR
		Type	Local
		At	WingSkeleton
		AnimPiv	Fore_FootR
		Part	:Celestial_Tendril.part
		Part	:Celestial_Tendril_Core.part
		POther	WingJointR
		LookAt	WingJointR
	End

	Event
		EName	WingMiddleL
		Type	Local
		At	WingSkeleton
		AnimPiv Fore_LLegL
		Part	:Celestial_Tendril.part
		Part	:Celestial_Tendril_Spread.part
		POther	FootLAnchor
	End

	Event
		EName	WingMiddleR
		Type	Local
		At	WingSkeleton
		AnimPiv Fore_LLegR
		Part	:Celestial_Tendril.part
		Part	:Celestial_Tendril_Spread.part
		POther	FootRAnchor
	End

	Event
		EName	WingBottomL
		Type	Local
		At	WingSkeleton
		AnimPiv	Fore_ToeL
		Part	:Celestial_Tendril_Consistent.part
		Part	:Celestial_Tendril_Spread2.part
		POther	ToeLAnchor
	End

	Event
		EName	WingBottomR
		Type	Local
		At	WingSkeleton
		AnimPiv	Fore_ToeR
		Part	:Celestial_Tendril_Consistent.part
		Part	:Celestial_Tendril_Spread2.part
		POther	ToeRAnchor
	End

End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	FLY
	DoMany		1

	Event
		EName	WingTrailL
		Type	Local
		At	Origin
		UNTIL	STEPLEFT
		Flags	OneAtATime
		CHILDFX	:FX_Wings_Celestial_Armor_CHILD.fx
	End
End

#############################################################

End