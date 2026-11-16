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
		EName	Prop_1
		Type	Local
		At	WepR
		BHVR	Behaviors/ClubColor.bhvr
		Geom    GEO_WepR_Croatoa_Jack_Club
	End
End

#############################################################

End