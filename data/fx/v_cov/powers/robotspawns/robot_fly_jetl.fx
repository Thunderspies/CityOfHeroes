#########################################################
##	
##		RobotSpawn Drop
##

#########################################################

FxInfo

##LifeSpan	200


############## Pulse 1  ###################

Condition	
	On 	Time
	Time 	0

	Event
		EName	EngineSounds
		Type	Start
		At	Hips

		Sound jetpack2_loop 70 70 .3
	End

	Event
		EName	EngineSounds01
		Type	Start
		At	Hips

		
		Sound glow7_loop 70 70 .66
	End
	
End


#########################################################
#########################################################
#########################################################
#################### Right foot jet #####################
#########################################################
#########################################################
#########################################################



Condition
	On 	Time
	Time 	0

	Event
		ENAME   p2
		Type	Local 
		At	FootL
		Bhvr	behaviors\FX_RobotSpawn_DropJetR.bhvr
		#Bhvr	V_COV\EnemyPowers\Arachnos\Arachnos_GunFlash.bhvr
		#Geom	FlameThrower
		#Geom	TESTING_Target
	End

	Event
		EName 	BlastR1
		Type	Local
		At	p2
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnos_GunFlash.bhvr
#		Geom	FX_MachineGunBurst
#		Part1	:RS_P_JetFlash.part
#		Part2	:RS_P_JetGlow.part
	End

	Event
		EName 	FlareR1
		Type	Local
		At	BlastR1
		#Altpiv 1
		#Bhvr	V_COV\EnemyPowers\Arachnos\Arachnos_GunFlash.bhvr
		Part1	:RS_P_flameA.part
		Part2	:RS_P_flameB.part
	End
End


##################################################

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	BlastR1a
		Type	Local
		At	p2

		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnos_GunFlash.bhvr
#		Geom	FX_MachineGunBurst
		Part1	:RS_P_JetFlash.part
		Part2	:RS_P_JetGlow.part
		Lifespan	16
	End

End

##################################################

Condition
	On 	Time
	Time 	2

	Event
		EName 	BlastR2
		Type	Local
		At	p2
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnos_GunFlash.bhvr
#		Geom	FX_MachineGunBurst
		Part1	:RS_P_JetFlash.part
		Part2	:RS_P_JetGlow.part
		Lifespan	16
	End

End

#################################################

Condition
	On 	Time
	Time 	4

	Event
		EName 	BlastR3
		Type	Local
		At	p2
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnos_GunFlash.bhvr
#		Geom	FX_MachineGunBurst
		Part1	:RS_P_JetFlash.part
		Part2	:RS_P_JetGlow.part
		Lifespan	16
	End

End



################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	SmokeR
		Type	Local
		At	P2
		
		
		Part	:RS_P_Smoke.part
	End

End


#########################################################
#########################################################
#########################################################
#########################################################
#########################################################
#########################################################
#########################################################


End
















		