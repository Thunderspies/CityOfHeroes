#########################################################
##	
########################################################
FxInfo

Input  
	Inpname	Root
End


##################################################################

Condition
	On	Time
	Time	0
	Event
		EName 	Extras
		Type	Start
		At	Root 
		Geom	Prop_speaker_extra
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End


End

Condition
	On	Death

	Event
		Type	Local 
		At	Root 
		LifeSpan 360
	End
End

End
##################################

