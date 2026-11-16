#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	1

	Event
		EName	Tentacle01
		Type	Local
		At	Back
		Bhvr	behaviors/WingScale_ANGEL_Female.bhvr
		BhvrOverride
			Scale		0.45 0.55 0.45
			PositionOffset	0 0.30 0.15
			PYRrotate 	-35 0.0 0
		End
		Anim	FX_wings_ANGEL
	End

	Event
		EName	Tentacle02
		Type	Local
		At	Waist
		Bhvr	behaviors/WingScale_ANGEL_Female.bhvr
		BhvrOverride
			Scale		0.60 0.55 0.55
			PositionOffset	0 0.1 -0.07
			PYRrotate 	30 0.0 0
		End
		Anim	FX_wings_ANGEL
	End

End

#############################################################

End