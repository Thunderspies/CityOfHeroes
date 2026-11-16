#########################################################
##	Tarantula Venom Bolt
##
#########################################################################################
########	Notes:
########	- Fire Prime1 a few frames before Prime 
########	- Fire Prime with attached particle emitters, spinning offset emiter, 
########	and a burst at firing blast origin
########	- Prime 1 hits killing just before prime hits to provide for an explosion
########	just prior to Prime impact, or visible impact
########	- If miss projectile explodes using parameters above
########
#########################################################################################

FxInfo
	
LifeSpan	85


Condition
	On	Time
	Time	0
	Event
		Type	Local
		At	Chest
		Sound TarantulaSpit 90 90 .94
	End
End


#################################################################################
############################  Emitter points  ###################################
#################################################################################

Condition
	On	Time
	Time	0

	Event
		EName	EmitterPos
		Type	Local
		At	Belt
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_VBoltEmitter.bhvr
#		Geom	Testing_Target
	End
End

#################################################################################
###################  P R I M E   decoy for early splat ##########################
#################################################################################

Condition
	On	Time
	Time	32

	Event
		EName	Prime1
		Type	Start
		At	EmitterPos
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_VBoltTravel.bhvr
#		Geom	Testing_Target
		Magnet	T_Chest
		LookAt	T_Chest
	End
End

#################################################################################
#################################  P R I M E  ###################################
#################################################################################

Condition
	On	Time
	Time	38

	Event
		EName	Prime
		Type	Start
		At	EmitterPos
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_VBoltTravel.bhvr
#		Geom	Testing_Target
		Magnet	T_Chest
		LookAt	T_Chest
	End

	Event
		EName	PrimeOff	# offset node that orbits projectile
		Type	Local
		At	Prime
	
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_VBoltOffSet.bhvr
#		Geom	Testing_Target
	End

#################################################################################

	Event
		EName	PrimeBall
		Type	Local
		At	Prime
		
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBolt.part
		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBolt01.part
		Part3	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBolt02.part
		PMagnet	PrimeBall
	End

	Event
		EName	PrimeBall01
		Type	Local
		At	PrimeOff
		
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBolt.part
		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBolt01.part
		Part3	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBolt02.part
		PMagnet	PrimeBall
	End

#################################################################################

	Event
		EName	PrimeDrips
		Type	Local
		At	Prime
		
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBoltDrips.part
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBoltDrips01.part
		Lifespan 20
		PMagnet	T_chest
	End

End


#################################################################################
##################################  B U R S T  ##################################
#################################################################################

Condition
	On	Time
	Time	38

	Event
		EName	PrimeSplat
		Type	Start
		At	EmitterPos
		
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBoltBurst01.part
		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBoltBurst.part
		#PMagnet	T_chest
	End
End

#################################################################################
#################################  T E S T S  ###################################
#################################################################################

Condition
	On	Prime1Hit

	Event
		EName	splat
		Type	PositOnly
		At	prime

		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstImpact01.part
		Part3	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBoltSplatImpact.part
		Part3	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstImpact02.part
		PMagnet	T_Chest
		Lifespan 15
	End

	Event
		EName	splatCloud
		Type	PositOnly
		At	prime

		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstCloud.part
#		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstBubbles.part
		PMagnet	T_Chest
		Lifespan 15
	End

	Event
		EName	Prime1
		Type	Destroy
	End

	Event
		EName	PrimeBall
		Type	Destroy
	End

	Event
		EName	PrimeBall01
		Type	Destroy
	End
End

Condition
	On	Prime1Hit

	Event
		EName	PrimeDrips
		Type	Destroy
	End
End

#Condition
#	On	Death
#
#	Event
#		EName	splat
#		Type	Local
#		At	prime
#
#		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBoltSplatImpact01.part
#		Part3	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBoltSplatImpact.part
#		Lifespan 15
#	End
#End

End			