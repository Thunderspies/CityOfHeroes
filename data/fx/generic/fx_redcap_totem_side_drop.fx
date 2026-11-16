#########################################################
##	
########################################################
FxInfo

LifeSpan 63

#### Totem linked to Mystic bone #############################

Condition
	On	Time
	Time	0
	Event
		EName 	Bucket
		Type	Local
		At	Mystic 
		Geom	redcaphenge_side
		Bhvr	Behaviors/totem_lift_side.bhvr
		LifeSpan 63
	End


End

####### Totem left behind  ####################################

Condition
	On	Death

	Event
		EName 	Bucket
		Type	Start
		At	Mystic 
		Geom	redcaphenge_sidedropped  
		Bhvr	Behaviors/GenericParticleFade.bhvr
		LifeSpan	3000
	End


End




End

