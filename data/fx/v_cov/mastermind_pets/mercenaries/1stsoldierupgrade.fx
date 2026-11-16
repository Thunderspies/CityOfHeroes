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
		Type	Local 
		At	WepR
		Sound Mercenary_soldierupgrade 60 60 .6
	End
End




Condition
	On 	Time
	Time 	30

	Event
		Type	Local 
		At	Hair
		GEOM	GEO_Hair_Mastermind_Helmet_01z
		
		Bhvr	Behaviors/Upgrade_Fade_UP.bhvr

	End
End

End	



			