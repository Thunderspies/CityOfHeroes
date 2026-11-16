#############################################################
## Ninja_katana_left.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Geom	GEO_WepL_Katana_3
		Sound swordshing 70 70 .6
	End
End

#############################################################

End