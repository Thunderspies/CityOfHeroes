#########################################################
##	Debris Trail FX
########################################################
FxInfo

######################################################################
Lifespan	200

Condition
	On 	Cycle
	Time 	6
	Chance	.6

	Event
		EName	trail
		Type	StartPositOnly
		At	Origin
		#geom	GEO_Slag_Golem_Junk_12
		geom	GEO_Slag_Golem_Junk_26
		geom	GEO_Slag_Golem_Junk_45
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
	Time 	4
	Chance	.5

	Event
		HardwareOnly
		EName	trail
		Type	StartPositOnly
		At	Origin
		#geom	GEO_Slag_Golem_Junk_12
		geom	GEO_Slag_Golem_Junk_26
		geom	GEO_Slag_Golem_Junk_45
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

