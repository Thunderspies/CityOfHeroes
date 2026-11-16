#############################################################
## Dual_Thug_Pistol.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	12

	Event
		Type	Local
		At	WepR
		Geom	GEO_WepR_Pistol_1
		Sound pistolout1 60 60 .28
	End

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors/Reverse_wep.bhvr
		Geom	GEO_WepR_Pistol_1
	End
End

#############################################################

End