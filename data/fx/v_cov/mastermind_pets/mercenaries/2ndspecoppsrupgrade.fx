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
		At	Emblem
		Geom	 GEO_Emblem_Mastermind_Padsz

		Bhvr	Behaviors/specUpgrade_Fade_UP.bhvr
		#Sound	shotgunout 60 50 .4
	End
	
	Event
		Type	Local 
		At	hair

		Geom	SpecOpps_earpiece

		Bhvr	Behaviors/Upgrade_Fade_UP.bhvr
		#Sound	shotgunout 60 50 .4
	End

	Event
		Type	Local 
		At	SpadR

		Geom	mercenaries_SpadR

		Bhvr	Behaviors/Upgrade_Fade_UP.bhvr
		#Sound	shotgunout 60 50 .4
	End
	
	Event
		Type	Local 
		At	SpadL

		Geom	mercenaries_SpadL

		Bhvr	Behaviors/Upgrade_Fade_UP2.bhvr
		#Sound	shotgunout 60 50 .4
	End
End

End	

			