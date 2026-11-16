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
	Time 	20

	Event
		Type	Local 
		At	emblem
		Geom	GEO_Emblem_Katana_Upgrade
		Bhvr	Behaviors/Katana_Fade_UP.bhvr
		Sound shotgunout 60 60 .4
	End
End

End	



			