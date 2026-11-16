#########################################################
##	jack
##
FxInfo
	
LifeSpan 0

Input  
	InpName	WepR
End

Flags    InheritAlpha

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Emblem
		Geom	GEO_Emblem_Ninja_Quiver
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		#Sound	shotgunout 60 50 .4
	End
End

End	



			