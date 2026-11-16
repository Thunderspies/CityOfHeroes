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
	Time 	200
	
	Event
		Type	Local 
		At	waist
		Geom	GEO_Mastermind_Belt_01z
		#Geom	GEO_Mastermind_Belt_02z

		Bhvr	Behaviors/Upgrade_Fade_UPa.bhvr
		
	End
	

End

End	



			