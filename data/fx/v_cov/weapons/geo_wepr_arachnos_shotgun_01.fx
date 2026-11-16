#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Geom	GEO_WepR_Arachnos_Shotgun_01

		Sound shotgunout 70 70 .34
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

#############################################################

End