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
		
		Geom	GEO_Hair_Hat_Bandanaz

		Bhvr	Behaviors/Upgrade_Fade_UP2.bhvr
		#Sound	shotgunout 60 50 .4
	End
End

End	



			