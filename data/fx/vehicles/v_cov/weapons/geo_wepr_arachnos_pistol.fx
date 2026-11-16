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
		Geom	GEO_WepR_Arachnos_Pistol

		Sound shotgunout 70 70 .12
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

End			