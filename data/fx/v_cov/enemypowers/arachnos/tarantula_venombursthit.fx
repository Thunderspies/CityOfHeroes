############################################################
##	Fireweapon
###########################################################
#########################################################################################
########	Notes:
########	- kick off a few bright particles at impact
########	- emit more small and large magnetized particles that spiral body
########	- emit gas
########
#########################################################################################



FxInfo

Flags	InheritAnimScale

Lifespan	200

#################################################################################
#############################  P R I M E    H I T  ##############################
#################################################################################

Condition
	On	Time
	Time	0
	Event
		EName	PoisonSplat
		Type	Local
		At	Chest

		Sound Spewhiss2 90 90 .77
		PMagnet	Hips
		Lifespan 10
	End
End

#################################################################################
########################  Continuous Flame Offsets  #############################
#################################################################################

Condition
	On	Time
	Time	7

	Event
		EName	Burst
		Type	Start
		At	Chest

		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstHitImpact.part
		PMagnet	Root
		POther	Waist
	End
End

Condition 
	On	Time
	Time	2

	Event	
		EName	Streaks
		Type	PositOnly
		At	UArmL
	
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidStreak.part
		POther	UArmR

	End

	Event	
		EName	Streaks01
		Type	PositOnly
		At	LArmR
	
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidStreakSM.part
		POther	RingR

	End

	Event	
		EName	Streaks02
		Type	PositOnly
		At	LArmL
	
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidStreakSM.part
		POther	RingL
	End
End

Condition
	On 	Time
	Time 	5

	Event
		EName 	Smoke
		Type	Local
		At	Chest

		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcid.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidDark.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidBubbles.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidBubblesdark.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidBubble.part
		POther	LLegL
	End	

	Event
		EName 	Smoke01
		Type	Local
		At	Chest

		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcid.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidDark.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidBubbles.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidBubblesdark.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidBubble.part
		POther	LLegR
	End	

	Event
		EName 	Smoke02
		Type	Local
		At	UArmL

		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcid.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidDark.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidBubbles.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidBubblesdark.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidBubble.part
		POther	UArmR
	End
End

End