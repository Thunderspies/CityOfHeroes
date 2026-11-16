#########################################################
##	
##		RobotSpawn Drop
##

#########################################################

FxInfo

LifeSpan	200




#########################################################
#########################################################
#########################################################
#################### left foot jet ######################
#########################################################
#########################################################
#########################################################



Condition
	On 	Time
	Time 	0

	Event
		ENAME   p1
		Type	Local 
		At	FootL
		Bhvr	behaviors\FX_RobotSpawn_DropJetR.bhvr
		#Bhvr	V_COV\EnemyPowers\Arachnos\Arachnos_GunFlash.bhvr
		#Geom	FlameThrower
		#Geom	TESTING_Target
	End

	Event
		EName 	BlastL1
		Type	Local
		At	p1
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnos_GunFlash.bhvr
#		Geom	FX_MachineGunBurst
#		Part1	:RS_P_JetFlash.part
#		Part2	:RS_P_JetGlow.part
	End

	Event
		EName 	FlareL1
		Type	Local
		At	BlastL1
		#Altpiv 1
		#Bhvr	V_COV\EnemyPowers\Arachnos\Arachnos_GunFlash.bhvr
		Part1	:RS_P_flameA.part
		Part2	:RS_P_flameB.part
	End

End

##################################################

Condition
	On 	Time
	Time 	16
	Event
		EName 	BlastL1a
		Type	Local
		At	p1
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnos_GunFlash.bhvr
#		Geom	FX_MachineGunBurst
		Part1	:RS_P_JetFlash.part
		Part2	:RS_P_JetGlow.part
		Lifespan	44
	End
End

##################################################

Condition
	On 	Time
	Time 	18

	Event
		EName 	BlastL2
		Type	Local
		At	p1
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnos_GunFlash.bhvr
#		Geom	FX_MachineGunBurst
		Part1	:RS_P_JetFlash.part
		Part2	:RS_P_JetGlow.part
		Lifespan	42
	End

End

#################################################

Condition
	On 	Time
	Time 	20

	Event
		EName 	BlastL3
		Type	Local
		At	p1
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnos_GunFlash.bhvr
#		Geom	FX_MachineGunBurst
		Part1	:RS_P_JetFlash.part
		Part2	:RS_P_JetGlow.part
		Lifespan	38
	End

End

#################################################

Condition
	On 	Time
	Time 	22

	Event
		EName 	BlastL4
		Type	Local
		At	p1
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnos_GunFlash.bhvr
#		Geom	FX_MachineGunBurst
		Part1	:RS_P_JetFlash.part
		Part2	:RS_P_JetGlow.part
		Lifespan	38
	End

End

#################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	SmokeL
		Type	Local
		At	P1
		
		
		Part	:RS_P_Smoke.part
	End

End



End
















		