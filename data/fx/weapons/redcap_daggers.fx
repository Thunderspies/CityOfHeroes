#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		EName	Prop_2
		Type	Local
		At	WepL
		Geom    GEO_WepR_Croatoa_Dagger
		Bhvr	Behaviors/Reverse_Wep.bhvr
		Bhvr	Behaviors/Reverse_Grip.bhvr
		Sound DaggersOut 66 66 .55
	End

	Event
		EName	Prop_4
		Type	Local
		At	WepR
		Geom    GEO_WepL_Croatoa_Dagger
		Bhvr	Behaviors/Reverse_Grip.bhvr

	End


End




End