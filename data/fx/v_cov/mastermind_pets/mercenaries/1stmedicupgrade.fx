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
	Time 	30


	Event
		Type	Local 
		At	hair
		
		Geom	GEO_Hair_Mastermind_Helmet_03z

		Bhvr	Behaviors/Upgrade_Fade_UP.bhvr
		#Sound	shotgunout 60 50 .4
	End
End

End	



			