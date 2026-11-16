#########################################################
## WeaponTest.fx - For testing animations compatability with equipped weapons
#########################################################

FxInfo

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	WepL
		Type	Local 
		At	WepL
		Anim	New_bow
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	WepR
		Type	Local 
		At	WepR
		Geom	GEO_WepR_Sword_1
	End
End

#########################################################

End