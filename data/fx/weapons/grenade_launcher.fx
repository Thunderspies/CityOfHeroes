#############################################################
## Grenade_Launcher.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\Weapon_Fade_UP.bhvr
		Geom	GEO_WepR_Grenade_Launcher_1
		Sound Shotgunout 80 80 .65
	End
End

#############################################################

End