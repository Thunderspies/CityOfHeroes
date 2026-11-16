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
		EName 	Prime
		Type	Local 
		At	LarmL
		Geom	GEO_SpadL_Ninja_Bow
		Bhvr	Behaviors/Ninja_Bow_Fade_UP.bhvr
		Sound shotgunout 60 60 .4
	End
End

End	



			