#########################################################
##	FireRing
##
FxInfo

LifeSpan	200	

#################################################################################
############################  Emitter points  ###################################
#################################################################################

Condition
	On 	Time
	Time 	11

	Event
		EName	EmitterPos
		Type	Local
		At	Belt
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_WillEmitter.bhvr
#		Geom	Testing_Target
	End

	Event
		EName	core
		Type	Posit
		At	EmitterPos
		Sound WillDom2 80 80 .8
	End

#################################################################################
################################  Blast Rings  ##################################
#################################################################################

	Event
		EName	core2
		Type	Local
		At	core
		
		part1	POWERS\Psionics\PsionicTeleRingsSmall.part
		part2	POWERS\Psionics\PsionicTeleRingzSmall.part		
		part1	POWERS\Psionics\PsionicTeleRingsSmall2.part
		part2	POWERS\Psionics\PsionicTeleRingzSmall2.part
		Lifespan 31
	End
End

Condition
	On 	Time
	Time 	25

	Event
		EName	core3
		Type	Local
		At	core
		
		part2	POWERS\Psionics\PsionicGlow1.part
		Lifespan 38
	End
End

#################################################################################
############################  Blast Projectile  #################################
#################################################################################

Condition
	On	Time
	Time	41

	Event
		EName	prime
		Type	Start
		At	Core

		Geom	FrontFacingLocalPivot
		#bhvr	behaviors/MedianProjectile3.bhvr
		bhvr	POWERS\Psionics\MedianProjectile3.bhvr
		Magnet	T_Head
		LookAt  T_Head
	End

	Event
		Type	Local
		At	Prime

		Part2	POWERS\Psionics\BlastStreakFront.part
		Part1	POWERS\Psionics\BlastStreakBig.part
	End
	
End

#################################################################################
#################################  K I L L A Z  #################################
#################################################################################

Condition
	On	PrimeHit
		
	Event
		EName	Prime
		Type	Destroy
	End
End

#########	#########	#########	#########	#########	#########


End			