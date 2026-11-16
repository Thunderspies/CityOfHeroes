#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	12

	Event
		EName 	right
		Type	Local
		At	WepR
		Geom	GEO_WepR_Pistol_Tactical
		Sound RES_Pistols_Equip 60 60 1
	End

	Event
		EName 	left
		Type	Local
		At	WepL
		Geom	GEO_WepR_Pistol_Tactical
		Bhvr	Behaviors/Reverse_wep.bhvr
	End
End

#############################################################

End