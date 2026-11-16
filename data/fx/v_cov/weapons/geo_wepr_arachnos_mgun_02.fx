#############################################################
## GEO_WepR_Arachnos_MGun_02.fx
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
		Geom	GEO_WepR_Arachnos_MGun_02
		Sound shotgunout 50 50 .22
	End
End

#############################################################

End