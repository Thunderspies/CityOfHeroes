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
			Scale			1.0 1.0 1.0
			PositionOffset		0.0 0.1 -0.55
	End
End

#############################################################

End