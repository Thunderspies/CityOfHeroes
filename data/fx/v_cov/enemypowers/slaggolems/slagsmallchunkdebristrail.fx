#########################################################
##	Debris Trail FX
########################################################
FxInfo

######################################################################
Lifespan	200

Condition
	On 	Cycle
	Time 	8
	Chance	.8

	Event
		EName	trail
		Type	StartPositOnly
		At	Origin
		geom	GEO_Slag_Golem_Junk_19
		geom	GEO_Slag_Golem_Junk_34
		geom	GEO_Slag_Golem_Junk_35
		geom	GEO_Slag_Golem_Junk_39
		geom	GEO_Slag_Golem_Junk_40
		geom	GEO_Slag_Golem_Junk_44
		bhvr	:SlagDebrisTrails.bhvr
		Lifespan	150
		LifespanJitter	30
				
	End
			

End


Condition
	On 	Cycle
	Time 	3
	Chance	.5

	Event
		HardwareOnly
		EName	trail
		Type	StartPositOnly
		At	Origin
		geom	GEO_Slag_Golem_Junk_19
		geom	GEO_Slag_Golem_Junk_34
		geom	GEO_Slag_Golem_Junk_35
		geom	GEO_Slag_Golem_Junk_39
		geom	GEO_Slag_Golem_Junk_40
		geom	GEO_Slag_Golem_Junk_44
		bhvr	:SlagDebrisTrails.bhvr
		Lifespan	150
		LifespanJitter	30
				
	End
			

End
##################################

