#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	12

	Event
		EName 	right
		Type	Local
		At	WepR
		Geom	GEO_WepR_Revolver_Magnum
		Sound pistolout1 60 60 .28
		BhvrOverride
			Scale	.8 .8 .8
		End
	End

	Event
		EName 	left
		Type	Local
		At	WepL
		Geom	GEO_WepR_Revolver_Magnum
		Bhvr	Behaviors/Reverse_wep.bhvr
		BhvrOverride
			PositionOffset	.1 0 0
			Scale	.8 .8 .8
		End
	End
End

#############################################################

End