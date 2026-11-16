#########################################################
##	jack
##
FxInfo

Input  
	InpName	WepR
End

Flags    InheritAlpha

Condition
	On 	Time
	Time 	10

	Event
		EName 	Prime
		Type	Local 
		At	WepR
		Geom	GEO_WepR_StraightKatana

		Sound swordshing 70 70 .6
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

End			