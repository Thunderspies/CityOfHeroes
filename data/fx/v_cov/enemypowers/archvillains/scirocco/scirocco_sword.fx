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
	Time 	9

	Event
		EName 	Prime
		Type	Local 
		At	WepR
		Geom	Swrd_Scirocoo_Blade
		Sound swordshing 100 100 .7
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

End		