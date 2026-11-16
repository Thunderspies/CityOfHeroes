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
		Type	Local
		At	Back
	//	Bhvr	behaviors/WingScale_DEMON_Male.bhvr
		Anim	FX_wings_SPIDERWEB
		BhvrOverride
			Alpha			0
			Scale			0.65 0.65 0.65
			PositionOffset		0.0 0.0 -0.08
	End
End

#############################################################

End