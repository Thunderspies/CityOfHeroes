#############################################################
## NemesisRifle.fx
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
		Geom	GEO_WepR_Nemesis_01
		Sound shotgunout 80 80 .6
	End
End

#############################################################

End