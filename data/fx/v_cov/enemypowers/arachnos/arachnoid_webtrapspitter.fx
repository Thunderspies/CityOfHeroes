#########################################################
##	
##

FxInfo

LifeSpan  200
Flags	InheritAnimScale

###############################################################################
###################   ClockWise rotating Targets   ############################
###############################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	TargetOrigin
		Type	Local
		At	Root
		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapTargetOrigin.bhvr
#		Geom	Testing_Target
	End
End

Condition
	On 	Time
	Time 	1

	Event
		EName	TargetParent
		Type	Start
		At	TargetOrigin
		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapTargetParentCL.bhvr
#		Geom	Testing_Target
	End
End

Condition
	On 	Time
	Time 	2

	Event
		EName	Target01
		Type	Local
		At	TargetParent
		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapTargets.bhvr
#		Geom	Testing_Target
	End

	Event
		EName	Target02
		Type	Local
		At	TargetParent
		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapTargets.bhvr
#		Geom	Testing_Target
	End

	Event
		EName	Target03
		Type	Local
		At	TargetParent
		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapTargets.bhvr
#		Geom	Testing_Target
	End
End

###############################################################################
#################   CounterClockWise rotating Targets   #######################
###############################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	TargetOrigin01
		Type	Local
		At	Root
		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapTargetOrigin01.bhvr
#		Geom	Testing_Target
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	TargetParentCCL
		Type	Start
		At	TargetOrigin01
		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapTargetParentCCL.bhvr
#		Geom	Testing_Target
	End
End

Condition
	On 	Time
	Time 	1

	Event
		EName	Target04
		Type	Local
		At	TargetParentCCL
		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapTargets.bhvr
#		Geom	Testing_Target
	End

	Event
		EName	Target05
		Type	Local
		At	TargetParentCCL
		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapTargets.bhvr
#		Geom	Testing_Target
	End

	Event
		EName	Target06
		Type	Local
		At	TargetParentCCL
		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapTargets.bhvr
#		Geom	Testing_Target
	End
End

###############################################################################
##########################   Head Emitter Spawn   #############################
###############################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Emitter
		Type	Local
		At	Head
		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_SpitEmit.bhvr
##		Geom	Testing_Target
#		Geom	LocalPivotz
		Sound arachnoidcocoonspit 80 80 .88
	End
End

################################################################################
############################   Build Up   ######################################
################################################################################

Condition
	On 	Time
	Time 	1

	Event
		EName	GatherWeb
		Type	Local
		At	Emitter

		Part1	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapWebGather.part
		Part2	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapWebGather01.part
#		Part3	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapWebGather02.part
		Part4	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapWebGather02.part
		Part5	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapWebGather03.part
		Magnet	Emitter
		LookAt	Emitter
		Lifespan 18
	End
End

################################################################################
########################  Spit Projectile Prime  ###############################
################################################################################

Condition
	On 	Time
	Time 	23

	Event
		EName	Prime
		Type	Start
		At	Emitter

		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_SpitTravel.bhvr
		Part2	V_COV\EnemyPowers\Arachnos\Arachnoid_P_SpitBallSM.part
		Part2	V_COV\EnemyPowers\Arachnos\Arachnoid_P_SpitBall.part
		Magnet	T_Root
		LookAt	T_Root
	End

	Event
		EName	PrimeStrands
		Type	Local
		At	Prime

		Part1	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitTail.part
		Part2	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitTail.part
		Part3	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitStrands.part
		Part4	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitStrands01.part
		Part5	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitStrands02.part
		Part5	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitStrands03.part
	End

	Event
		EName	WebTrail
		Type	Local
		At	Prime

		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_WebScale.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebTrail.part
	End

	Event
		EName	WebTrail01
		Type	Local
		At	Prime
		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_SpitTrailOffset.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebTrail01.part
	End
End	

Condition
	On	Time
	Time	32

	Event
		EName	WebCloud
		Type	Local
		At	Prime

		Part1	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapCloud.part
		Magnet	Prime
		LookAt	Prime
	End
End

Condition
	On	PrimeHit

	
	Event
		EName	WebCloudBurst
		Type	Start
		At	Prime

		Part1	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapCloudBurst.part
	End

	Event
		EName	WebCloud
		Type	Destroy
	End


End

###############################################################################
############################   WebsMiss Targets   #############################
###############################################################################

Condition
	On 	Time
	Time 	23

	Event
		EName	Prime1
		Type	Start
		At	Emitter

		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_SpitTravel.bhvr
		#Geom	Testing_Target
		Magnet	Target01
		LookAt	Target01
	End

	Event
		EName	Prime2
		Type	Start
		At	Emitter

		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_SpitTravel.bhvr
		#Geom	Testing_Target
		Magnet	Target02
		LookAt	Target02
	End

	Event
		EName	Prime3
		Type	Start
		At	Emitter

		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_SpitTravel.bhvr
		#Geom	Testing_Target
		Magnet	Target03
		LookAt	Target03
	End

	Event
		EName	Prime4
		Type	Start
		At	Emitter

		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_SpitTravel.bhvr
		#Geom	Testing_Target
		Magnet	Target04
		LookAt	Target04
	End

	Event
		EName	Prime5
		Type	Start
		At	Emitter

		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_SpitTravel.bhvr
		#Geom	Testing_Target
		Magnet	Target05
		LookAt	Target05
	End
End

###############################################################################
##############################   Webs Shoot   #################################
###############################################################################

Condition
	On 	Time
	Time 	23

	Event
		EName	SwarmTarget01
		Type	Local
		At	Prime1

		Part1	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitStrands.part
		Part2	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitStrands01.part
		Part3	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitStrands02.part
		Part4	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitStrands03.part
		Magnet	Target01
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	24

	Event
		EName	SwarmTarget02
		Type	Local
		At	Prime2

		Part3	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitStrands.part
		Part4	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitStrands01.part
		Part5	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitStrands02.part
		Part5	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitStrands03.part
		Magnet	Target06
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	24

	Event
		EName	SwarmTarget03
		Type	Local
		At	Prime3

		Part3	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitStrands.part
		Part4	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitStrands01.part
		Part5	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitStrands02.part
		Part5	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitStrands03.part
		Magnet	Target02
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	24

	Event
		EName	SwarmTarget03
		Type	Local
		At	Prime4

		Part3	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitStrands.part
		Part4	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitStrands01.part
		Part5	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitStrands02.part
		Part5	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitStrands03.part
		Magnet	Target05
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	24


	Event
		EName	SwarmTarget05
		Type	Local
		At	Prime5

		Part3	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitStrands.part
		Part4	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitStrands01.part
		Part5	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitStrands02.part
		Part5	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitStrands03.part
		Magnet	Target04
		Lifespan 30
	End


	Event
		EName	SwarmTarget06
		Type	Local
		At	Prime

		Part3	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitStrands.part
		Part4	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitStrands01.part
		Part5	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitStrands02.part
		Part5	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapSpitStrands03.part
		Magnet	Target06
		Lifespan 30
	End
End

###############################################################################
#################################   Impact   ##################################
###############################################################################

Condition
	On 	PrimeHit
	
	Event
		EName 	PrimeSpawn
		Type	Local
		At	Prime

	End

	Event
		EName 	Prime
		Type	Destroy
	End


	Event
		EName 	Trails
		Type	Destroy
	End


End

End