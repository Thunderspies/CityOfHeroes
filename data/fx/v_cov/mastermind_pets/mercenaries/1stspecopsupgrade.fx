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
		At	hair
		Geom	GEO_Eyes_Mastermind_Goggles_01z
		
		Bhvr	Behaviors/Upgrade_Fade_UP.bhvr
		#Sound	shotgunout 60 50 .4
	End
	
	Event
		Type	Local 
		At	hair

		Geom	GEO_Hair_Mastermind_Helmet_02z
		#Geom	GEO_Mastermind_Belt_02z
		
		Bhvr	Behaviors/Upgrade_Fade_UP.bhvr
		#Sound	shotgunout 60 50 .4
	End
End

End	



			