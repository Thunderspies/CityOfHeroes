#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	9

	Event
		EName 	Pickaxe_R
		Type	Local
		At	WepR
		Geom	GEO_WepR_Shovel_03
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Sound maceout 60 60 .5
	End


End

#############################################################

End