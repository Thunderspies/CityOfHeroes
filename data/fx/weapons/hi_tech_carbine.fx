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
		EName 	Prime
		Type	Local
		At	WepR
		Geom	GEO_WepR_Machine_Gun_2
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Sound shotgunout 60 60 .44
	End
End

#############################################################

End