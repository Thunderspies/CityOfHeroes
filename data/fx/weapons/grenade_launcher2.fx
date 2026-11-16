#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	4

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Geom	GEO_WepR_Grenade_Launcher_2
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Sound Shotgunout 80 80 .65
	End
End

#############################################################

End