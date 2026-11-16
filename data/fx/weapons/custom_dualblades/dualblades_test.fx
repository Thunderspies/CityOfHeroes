#########################################################
## CustomWeapon_DualBlades.fx
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepR
		Sound swordshing 100 100 0.7
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepL
		Bhvr	behaviors\Reverse_Wep.bhvr
		Geom	GEO_WepR_Katana_2
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepR
		Geom	GEO_WepR_Katana_2
	End
End

#############################################################

End