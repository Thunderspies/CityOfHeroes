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
		Type Local
		At root

		Sound jetpack2_loop 70 70 .15
	End

	Event
		Type Local
		At root
		Sound glow7_loop 70 70 .45
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
		At	LlegL
		Bhvr	behaviors\FX_RobotSpawn_DropJetR.bhvr
                        BhvrOverride
                                    Behavior
                                    PositionOffset	0 -2.5 -.5
			End
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
















		