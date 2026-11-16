#########################################################
## Test Dual Blades
#########################################################

FxInfo

Flags InheritAlpha ##InheritAnimScale

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	WepR
		Sound swordshing 100 100 .7
	End

	Event
		Type	Local
		At	WepR
		Geom	GEO_WepR_Scimitar01
	End

	Event
		EName	LeftHandWeaponHookup
		Type	Local
		At	WepL
		Bhvr	behaviors\Reverse_Wep.bhvr
		Geom	GEO_WepR_Scimitar01
	End
End

#############################################################

End