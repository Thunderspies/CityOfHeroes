#########################################################
##	
########################################################
FxInfo

Input  
	Inpname	WepR
End


##################################################################

Lifespan 160

Condition
	On	Time
	Time	22


	Event
		EName 	pipe_ground
		Type	Local 
		At	WepR 
		Geom	GEO_WepR_prop_pipe  
		Bhvr	Behaviors/GenericParticleFade.bhvr

	End

End

Condition
	On 	Time
	Time 	118


	Event
		EName	pipe_ground
		Type	Destroy
		
	End

End


End
##################################

