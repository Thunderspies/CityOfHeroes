#########################################################
##	
##

FxInfo

LifeSpan  200
Flags	InheritAnimScale

###############################################################################
##################################   Spit   ###################################
###############################################################################

Condition
	On 	Time
	Time 	18

	Event
		EName	Emitter
		Type	Local
		At	Belt
		
		Bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_CocoonEmit.bhvr
#		Geom	Testing_Target
		#SOund	poisonprojectile 80 30 .88
		Sound coccoonlaunch 90 90 .88
	End
End

################################################################################
############################   Build Up   ######################################
################################################################################

Condition
	On 	Time
	Time 	1

	Event
		EName	GatherWebGas
		Type	Local
		At	Emitter

		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_CocoonGatherBall.part
		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_CocoonGatherGas.part
		Part4	V_COV\EnemyPowers\Arachnos\Tarantula_P_CocoonGather.part
		PMagnet	Emitter
		Lifespan 12
	End
End

################################################################################
########################  Spit Projectile Prime  ###############################
################################################################################

Condition
	On 	Time
	Time 	20

	Event
		EName	Prime
		Type	Start
		At	Emitter

		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_SpitTravel.bhvr
		Part2	V_COV\EnemyPowers\Arachnos\Arachnoid_P_SpitBallSM.part
		Part2	V_COV\EnemyPowers\Arachnos\Arachnoid_P_SpitBall.part
		Geom	Testing_Target
		Magnet	T_Root
		LookAt	T_Root
	End

	Event
		EName	WebTrail
		Type	Local
		At	Prime

		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_WebScale.bhvr
#		Part1	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebTrail.part
	End

	Event
		EName	WebTrail01
		Type	Local
		At	Prime
		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_SpitTrailOffset.bhvr
#		Part1	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebTrail01.part
		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_CocoonTrail.part
		PMagnet	Prime
	End

	
End

###############################################################################
#################################   Impact   ##################################
###############################################################################

Condition
	On 	PrimeHit
	
	Event
		EName 	Prime
		Type	Destroy
	End


	Event
		EName 	Trails
		Type	Destroy
	End

	Event
		EName	Splash
		Type	Start
		At	T_Root
		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_Splash.bhvr
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_Splash.part
#		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_Splash01.part
#		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_Splash02.part

		LifeSpan	1		
	End

	Event
		EName   RadiusFxScale
		Type	local
		At	T_Root
		Bhvr	powers/firecontrol/Inferno.bhvr
	End


	Event
		EName 	Prime1
		Type	start 
		At	RadiusFxScale
		bhvr    WEAPONs/Bow/TrickArrow/stickyBubble.bhvr
		Part	WEAPONs/Bow/TrickArrow/StickyYellow2.part
		Part	WEAPONs/Bow/TrickArrow/StickyGlows.part
		LifeSpan	30
		
	End		

End

End				