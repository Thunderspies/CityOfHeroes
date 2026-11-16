#########################################################
##	
########################################################
FxInfo



##################################################################

LifeSpan 84  ##88

#### Totem linked to Mystic bone #############################

Condition
	On	Time
	Time	0
	Event
		EName 	Bucket
		Type	Local
		At	Mystic 
		Geom	redcaphenge
		Bhvr	Behaviors/totem_lift.bhvr
		LifeSpan 84  ##88
	End


End

####### Totem left behind  ####################################

Condition
	On	Death

	Event
		EName 	Bucket
		Type	Start
		At	Mystic 
		Geom	redcaphenge_remain
		Bhvr	Behaviors/GenericParticleFade.bhvr
		LifeSpan	3000
	End


End

End
##################################

