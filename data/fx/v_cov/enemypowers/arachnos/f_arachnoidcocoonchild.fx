#########################################################
##	geometry for Arachnoid Spin Cocoon PL
########################################################
FxInfo

####  start by spawning the cocoon geometry ####

Condition
	On	Time
	Time	0

	Event
		EName	CocoonPosit
		Type	Start
		At	Origin

		bhvr	V_COV\EnemyPowers\Arachnos\B_ArachnoidCocoonChild.bhvr

		Geom	Arachnoid_Cocoon
#		Lifespan 1
	End
End

Condition
	On	Cycle
	Time	200

	Event
		EName	CocoonGlow
		Type	Local
		At	CocoonPosit
		
		bhvr	V_COV\EnemyPowers\Arachnos\B_ArachnoidCocoonFade.bhvr
		Geom	Arachnoid_Cocoon
		Lifespan 100
	End
End

Condition
	On	Death

	Event
		EName	Cocoon
		Type	Start 
		At	Root 
		LifeSpan 360
	End
End




End