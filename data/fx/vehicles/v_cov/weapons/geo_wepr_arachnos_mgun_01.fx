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
		Geom	GEO_WepR_Arachnos_MGun_01

		Sound Shotgunout 70 70 .34
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

End			