#########################################################
##	
########################################################
FxInfo



##################################################################

Condition
	On	Time
	Time	0
	Event
		EName 	Destroyed_Chamber
		Type	Start
		At	Root 
		Geom	Prop_bucket
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End


End

#Condition
#	On	Death
#
#	Event
#		Type	Local 
#		At	Root 
#		LifeSpan 360
#	End
#End

End
##################################

