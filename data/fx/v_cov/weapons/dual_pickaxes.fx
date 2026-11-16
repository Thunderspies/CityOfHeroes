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
		Geom	GEO_WepR_Pick_Axe_02
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Sound axeout 50 50 .4
	End

	Event
		EName 	Pickaxe_L
		Type	Local
		At	WepL
		Geom	GEO_WepL_Pick_Axe_02
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End


End

End