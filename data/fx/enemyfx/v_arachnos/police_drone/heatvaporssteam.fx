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


###############################################################################


###############################################################################

	Event
		EName	Vapors
		Type	local
		At	chest
		geom	HeatWaveSphere2
		bhvr	:Scale.bhvr
		#Lookat	camera
	End

	Event
		EName	Steam
		Type	Local 
		At	Root
		part   :Steam.part
	End

End

Condition
	On 	triggerbits
	Triggerbits	death

	Event
		EName 	Vapors
		Type	Destroy
	End
	
	Event
		EName 	Steam
		Type	Destroy
	End

End

End				