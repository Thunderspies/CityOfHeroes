#########################################################
##	FireAura
FxInfo

##################################

Condition
	On	Time
	Time	0
	
########################################################################
##SOUND
##########################################


	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepR
		Sound Firehand_loop 100.0 100.0 .45
		
	
	End

###############################################################################

	Event
		Type	Local
		At	root
		geom	HeatWaveSphere
		bhvr	:Scale.bhvr
	End

	Event
		Type	Local 
		At	neck
		part   :Rainfall.part
				
	End

	Event
		Type	Local 
		At	UARML
		part   :RainfallArms.part

	End

	Event
		Type	Local 
		At	UARMR
		part   :RainfallArms.part

	End

	Event
		Type	Local 
		At	WepL
		part   :RainfallArms.part
		
	End

		Event
		Type	Local 
		At	WepR
		part   :RainfallArms.part
		
	End
	
	Event
		Type	Local 
		At	LLEGL
		part   :RainfallArms2.part
	End

	Event
		Type	Local 
		At	LLEGR
		part   :RainfallArms2.part
	End

	Event
		Type	Local 
		At	Root
		part   :Steam.part
	End

End

End				