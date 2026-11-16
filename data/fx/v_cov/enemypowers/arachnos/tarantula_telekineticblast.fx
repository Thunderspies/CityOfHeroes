#########################################################
##	FireRing
##
FxInfo

LifeSpan	150	

#################################################################################
############################  Emitter points  ###################################
#################################################################################

Condition
	On 	Time
	Time 	24

	Event
		EName	EmitStart
		Type	Local
		At	Belt

		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_TeleEmitter.bhvr
#		Geom	PsiTele
		Sound telekinetic2 80 80 .8
		Lifespan 18	
	End

	Event
		EName	Prime
		Type	Start
		At	EmitStart

		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_TeleTracker.bhvr
#		Geom	Testing_Target
		Magnet	Target		
	End

	Event
		EName	core
		Type	Start
		At	EmitStart

		Sound telekinetic2 80 80 .8
		Lifespan 18	
	End


	Event
		EName	core2
		Type	Local
		At	core
		
		part1	POWERS\Psionics\PsionicTeleRingsSmall.part
		#part2	POWERS\Psionics\PsionicTeleRingzSmall.part
		Lifespan 18
	End

End

Condition
	On 	Time
	Time 	25

	Event
		EName	core3
		Type	Local
		At	core
		
		part2	POWERS\Psionics\PsionicGlow2.part
		Lifespan 95		
	End
End


End			