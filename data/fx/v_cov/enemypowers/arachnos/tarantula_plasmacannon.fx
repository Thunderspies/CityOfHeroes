#########################################################################################
#####################  Fire Tarantula Plasma Cannon Power  ##############################
#########################################################################################

#########################################################################################
########	Notes:
########
########
#########################################################################################

FxInfo

LifeSpan	200

#################################################################################
#######################  E M I T T E R   S P A W N S  ###########################
#################################################################################

Condition
	On 	Time
	Time 	0
	
	Event
		Type	Local
		At 	origin
		Sound robotlaserrifleburst4 100 100 1.0
	End
End




Condition
	On 	Time
	Time 	35
	
	Event
		Type	Local
		At 	origin
		Sound flamebolt4 100 100 .79
	End
End



Condition
	On	Time
	Time	0
	
	Event
		EName	Prime_Emitter
		Type	Local
		At	Belt

		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PlasmaOffset.bhvr
#		Geom	Testing_Target
	End
End

#Condition
#	On	Time
#	Time	1
#	
#	Event
#		EName	Emitter_Spin00
#		Type	Local
#		At	Prime_Emitter
#
#		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PlasmaBuildUp00.bhvr
##		Geom	Testing_Target
#	End
#
#	Event
#		EName	Emitter_Spin01
#		Type	Local
#		At	Prime_Emitter
#
#		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PlasmaBuildUp01.bhvr
##		Geom	Testing_Target
#	End
#
#End

#################################################################################
############################  Build Up Sparks Right Arm  ########################
#################################################################################

Condition
	On	Time
	Time	1

	Event
		EName	ToeR_Sparks
		Type	Local 
		At	Fore_ToeR

		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_PlasmaSparkBU.part
		Lifespan 10
		#POther	Fore_FootR
		PMagnet	Prime_Emitter
		#LookAt	Emitter_Spin01
	End

	Event
		EName	ToeR_Sparks01
		Type	Local 
		At	Fore_ToeR

		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_PlasmaSparkBU.part
		Lifespan 10
		POther	Fore_FootR
		PMagnet	Prime_Emitter
		#LookAt	Emitter_Spin01
	End

	Event
		EName	FootR_Sparks
		Type	Local 
		At	Fore_FootR

		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_PlasmaSparkBU.part
		Lifespan 10
		POther	Fore_LLegR
		PMagnet	Prime_Emitter
		#LookAt	Emitter_Spin01
	End
	
	Event
		EName	ClawR_Sparks
		Type	Local 
		At	Fore_LLegR

		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_PlasmaSparkBU.part
		Lifespan 10
		POther	ClawR
		PMagnet	Prime_Emitter
		#LookAt	Emitter_Spin01
	End
End

#################################################################################
############################  Build Up Sparks Left Arm  #########################
#################################################################################

Condition
	On	Time
	Time	1

	Event
		EName	ToeL_Sparks
		Type	Local 
		At	Fore_ToeL

		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_PlasmaSparkBU.part
		Lifespan 15
		#POther	Fore_FootL
		PMagnet	Prime_Emitter
		#LookAt	Emitter_Spin01
	End

	Event
		EName	ToeL_Sparks01
		Type	Local 
		At	Fore_ToeL

		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_PlasmaSparkBU.part
		Lifespan 15
		POther	Fore_FootL
		PMagnet	Prime_Emitter
		#LookAt	Emitter_Spin01
	End

	Event
		EName	FootL_Sparks
		Type	Local 
		At	Fore_FootL

		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_PlasmaSparkBU.part
		Lifespan 15
		POther	Fore_LLegL
		PMagnet	Prime_Emitter
		#LookAt	Emitter_Spin01
	End

	Event
		EName	ClawL_Sparks
		Type	Local 
		At	Fore_LLegL

		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_PlasmaSparkBU.part
		Lifespan 15
		POther	ClawL
		PMagnet	Prime_Emitter
		#LookAt	Emitter_Spin01
	End
End

#################################################################################
#####################  E M I T T E R   Build Up and Burst  ######################
#################################################################################


Condition
	On	Time
	Time	10

	Event
		EName	BuildUpBall
		Type	Local 
		At	Prime_Emitter

		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_PlasmaBuildUp01.part
		Part4	V_COV\EnemyPowers\Arachnos\Tarantula_P_PlasmaSparkBU.part
		Lifespan 26
		PMagnet	Prime_Emitter
		#LookAt	Emitter_Spin01
	End
End


Condition
	On	Time
	Time	38

	Event
		EName	BURST
		Type	PositOnly 
		At	Prime_Emitter

		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_PlasmaBuildUp.part
		Lifespan 2
		#Magnet	Emitter_Spin00
		#LookAt	Emitter_Spin01
	End

End

#################################################################################
######################  P R I M E   P R O J E C T I L E  ########################
#################################################################################

Condition
	On	Time
	Time	39

	Event
		EName 	Prime
		Type	Start 
		At	Prime_Emitter
		
		Bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PlasmaTravel.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_PlasmaShotFlame00.part		# Blue Big 
		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_PlasmaShotFlame01.part		# Blue Core
		Part4	V_COV\EnemyPowers\Arachnos\Tarantula_P_PlasmaSpark.part			# Sparks
#		Geom	Testing_Target
		Magnet	T_Hips
		LookAt	T_Hips
	End


#################################################################################
###################  Prime Spinning Target Counter ClockWise  ###################
#################################################################################

	Event	
		EName	PrimeSpinCCW
		Type	Local
		At	Prime	

		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PlasmaSpinCCW.bhvr
#		Geom	Testing_Target
	End

	Event
		EName 	PrimeSpin00
		Type	Local 
		At	PrimeSpinCCW
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PlasmaSpinOffset00.bhvr
#		Geom	Testing_Target
	End

	Event
		EName 	PrimeSpin01
		Type	Local 
		At	PrimeSpinCCW
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PlasmaSpinOffset01.bhvr
#		Geom	Testing_Target
	End

#################################################################################
#######################  Prime Spinning Target ClockWise  #######################
#################################################################################

	Event	
		EName	PrimeSpinCW
		Type	Local
		At	Prime	

		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PlasmaSpinCW.bhvr
#		Geom	Testing_Target
	End

	Event
		EName 	PrimeSpin02
		Type	Local 
		At	PrimeSpinCW
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PlasmaSpinOffset00.bhvr
#		Geom	Testing_Target
	End

	Event
		EName 	PrimeSpin03
		Type	Local 
		At	PrimeSpinCW
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PlasmaSpinOffset01.bhvr
#		Geom	Testing_Target
	End

#################################################################################
###########################   Prime Spinning Emitters  ##########################
#################################################################################

	Event
		EName 	PrimeSpinEmit00
		Type	Local 
		At	PrimeSpin00
		
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_PlasmaSparkBU.part
		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_PlasmaShotMag00.part
		PMagnet	PrimeSpin00
		#LookAt	PrimeSpin00
	End

	Event
		EName 	PrimeSpinEmit01
		Type	Local 
		At	PrimeSpin01
		
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_PlasmaSparkBU.part
		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_PlasmaShotMag00.part
		PMagnet	PrimeSpin01
		#LookAt	PrimeSpin01
	End

		Event
		EName 	PrimeSpinEmit00
		Type	Local 
		At	PrimeSpin02
		
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_PlasmaSparkBU.part
		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_PlasmaShotMag00.part
		PMagnet	PrimeSpin02
		#LookAt	PrimeSpin02
	End

	Event
		EName 	PrimeSpinEmit01
		Type	Local 
		At	PrimeSpin03
		
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_PlasmaSparkBU.part
		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_PlasmaShotMag00.part
		PMagnet	PrimeSpin03
		#LookAt	PrimeSpin03
	End

End

#################################################################################
##############################   Prime K I L L A Z   ############################
#################################################################################

Condition
	On	PrimeHit
	
	Event
		Ename	Prime
		Type	Destroy
	End
	
	Event
		Ename	PrimeMagnet
		Type	Destroy
	End

End


	
	
End				