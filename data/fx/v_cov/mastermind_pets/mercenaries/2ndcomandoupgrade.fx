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

#	Event
#		Type	Local 
#		At	hair
#		Geom	 GEO_Mastermind_Belt_02
#
#		Bhvr	Behaviors/Upgrade_Fade_UP.bhvr
#		#Sound	shotgunout 60 50 .4
#	End
#	
#	Event
#		Type	Local 
#		At	eyes
#		Geom	 GEO_Eyes_Mastermind_Goggles_01
#
#		Bhvr	Behaviors/Upgrade_Fade_UP.bhvr
#		#Sound	shotgunout 60 50 .4
#	End
	
	Event
		Type	Local 
		At	waist
		
		Geom	GEO_Mastermind_Belt_03z

		Bhvr	Behaviors/Upgrade_Fade_UPb.bhvr
		#Sound	shotgunout 60 50 .4
	End
End

End	



			