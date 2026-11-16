############################################################
##	Fireweapon
###########################################################
#########################################################################################
########	Notes:
########
########	- First events spawn offset positions for flames on arm/leg,
########	uncomment "Geom	  Testing_Target" to see positions
########	- Each leg spawns bursts that step down from hip to tip when igninted
########	10 frame lifespan on burst, including joints
########	- Then each leg plays a continuous loop of flame from hip to tip,
########	joints excluded, end when global lifespan completes
########
#########################################################################################



FxInfo

Flags	InheritAnimScale

Lifespan	180

#################################################################################
########################  Continuous Flame Offsets  #############################
#################################################################################
Condition
	On	Time
	Time	0

	Event
		EName	LLegROffSet
		Type	Local
		At	Fore_LLegR

		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_BurnOffSet_R1.bhvr
#		Geom	Testing_Target
	End

	Event
		EName	ClawROffSet
		Type	Local
		At	ClawR

		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_BurnOffSet_R0.bhvr
#		Geom	Testing_Target
	End

	Event
		EName	LLegLOffSet
		Type	Local
		At	Fore_LLegL

		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_BurnOffSet_R1.bhvr
#		Geom	Testing_Target
	End

	Event
		EName	ClawLOffSet
		Type	Local
		At	ClawL

		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_BurnOffSet_L0.bhvr
#		Geom	Testing_Target
	End

	Event	
		EName	ChildGlow
		Type	Local
		At	Root

		Childfx	V_COV\EnemyPowers\Arachnos\Tarantula_PoisonSlash_Child.fx
		Lifespan 180
	End
End

#################################################################################
###################################  Right Arm  #################################
#################################################################################

Condition			##### Hips #####
	On 	Time
	Time 	5

	Event
		EName	FireHipR_Burst
		Type	Local 
		At	ClawROffSet
		
#		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstImpact01.part
#		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_PSlashBurst.part
		Part3	V_COV\EnemyPowers\Arachnos\Tarantula_P_PSlashBurst01.part
		POther	ClawROffSet
		PMagnet	ClawROffset
		Lifespan 5
	End
End


Condition			##### Hips #####
	On 	Time
	Time 	6

	Event
		EName	FireLLegR_Burst
		Type	Local 
		At	Fore_LLegR
		
#		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstImpact01.part
#		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_PSlashBurst.part
		Part3	V_COV\EnemyPowers\Arachnos\Tarantula_P_PSlashBurst01.part
		POther	ClawROffSet
		PMagnet	Fore_LLegR
		Lifespan 5
	End

	Event
		EName	FireLLegR
		Type	Local 
		At	LLegROffSet
		
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidStreakSM.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidStreak.part
#		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcid.part
#		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidDark.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidBubbles.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidBubblesdark.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidBubble.part
		POther	ClawROffSet
		Lifespan	165
	End
End

Condition
	On 	Time
	Time 	7

	Event
		EName	FireKneeR_Burst
		Type	Local 
		At	Fore_LLegR
		
#		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_PSlashBurst.part
#		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_PSlashBurst01.part
#		POther	ClawLOffSet
		Lifespan 6
	End
End
###################################  Right Arm  #################################

Condition			##### Mid Leg #####
	On 	Time
	Time 	8

	Event
		EName	FireFootR_Burst
		Type	Local 
		At	Fore_FootR
		
#		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstImpact01.part
#		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_PSlashBurst.part
		Part3	V_COV\EnemyPowers\Arachnos\Tarantula_P_PSlashBurst01.part
		POther	Fore_LLegR
		PMagnet	Fore_FootR
		Lifespan 7
	End

	Event
		EName	FireFootR
		Type	Local 
		At	Fore_FootR
		
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidStreakSM.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidStreak.part
#		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcid.part
#		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidDark.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidBubbles.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidBubblesdark.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidBubble.part
		POther	LLegROffSet
		Lifespan	172
	End
End

Condition
	On 	Time
	Time 	9

	Event
		EName	FireFootR_Burst
		Type	Local 
		At	Fore_FootR
		
#		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_PSlashBurst.part
#		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_PSlashBurst01.part
#		POther	ClawLOffSet
		Lifespan 8
	End
End


###################################  Right Arm  #################################

Condition			##### Low Leg #####
	On 	Time
	Time 	10

	Event
		EName	FireFootR_Burst
		Type	Local 
		At	Fore_ToeR
		
#		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstImpact01.part
#		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_PSlashBurst.part
		Part3	V_COV\EnemyPowers\Arachnos\Tarantula_P_PSlashBurst01.part
		POther	Fore_FootR
		PMagnet	Fore_ToeR
		LifeSpan	8
	End

	Event
		EName	FireToeR
		Type	Local 
		At	Fore_ToeR
		
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidStreakSM.part
#		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcid.part
#		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidDark.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidBubbles.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidBubblesdark.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidBubble.part
		Sound Firehand_loop 100.0 100.0 .55
		POther	Fore_FootR
		Lifespan	180
	End
End


Condition
	On 	Time
	Time 	11

	Event
		EName	FireFootR_Burst
		Type	Local 
		At	Fore_ToeR
		
#		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_PSlashBurst.part
		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_PSlashBurst01.part
#		POther	ClawLOffSet
		LifeSpan	9
	End
End

#################################################################################
###################################  Left Arm  #################################
#################################################################################

Condition			##### Hips #####
	On 	Time
	Time 	6

	Event
		EName	FireLLegL_Burst
		Type	Local 
		At	Fore_LLegL
		
#		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstImpact01.part
#		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_PSlashBurst.part
		Part3	V_COV\EnemyPowers\Arachnos\Tarantula_P_PSlashBurst01.part
		POther	ClawLOffSet
		PMagnet	Fore_LLegL
		Lifespan 5
	End

	Event
		EName	FireLLegL
		Type	Local 
		At	LLegLOffSet
		
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidStreakSM.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidStreak.part
#		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcid.part
#		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidDark.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidBubbles.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidBubblesdark.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidBubble.part
		POther	ClawLOffSet
		Lifespan	165
	End
End

Condition
	On 	Time
	Time 	7

	Event
		EName	FireKneeL_Burst
		Type	Local 
		At	Fore_LLegL
		
#		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_PSlashBurst.part
		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_PSlashBurst01.part
#		POther	ClawLOffSet
		Lifespan 6
	End
End

###################################  Left Arm  #################################

Condition		         ##### Mid Leg #####
	On 	Time
	Time 	8

	Event
		EName	FireFootL_Burst
		Type	Local 
		At	Fore_FootL
		
#		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstImpact01.part
#		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_PSlashBurst.part
		Part3	V_COV\EnemyPowers\Arachnos\Tarantula_P_PSlashBurst01.part
		POther	Fore_LLegL
		PMagnet	Fore_FootL
		Lifespan 7
	End

	Event
		EName	FireFootL
		Type	Local 
		At	Fore_FootL
		
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidStreakSM.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidStreak.part
#		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcid.part
#		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidDark.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidBubbles.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidBubblesdark.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidBubble.part
		POther	LLegLOffSet
		Lifespan	172
	End
End

Condition
	On 	Time
	Time 	9

	Event
		EName	FireFootL_Burst
		Type	Local 
		At	Fore_FootL
		
#		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_PSlashBurst.part
		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_PSlashBurst01.part
#		POther	ClawLOffSet
		Lifespan 8
	End
End


###################################  Left Arm  #################################

Condition		      ##### Lower  Leg #####
	On 	Time
	Time 	8

	Event
		EName	FireFootL_Burst
		Type	Local 
		At	Fore_ToeL
	
#		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstImpact01.part
#		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_PSlashBurst.part
		Part3	V_COV\EnemyPowers\Arachnos\Tarantula_P_PSlashBurst01.part
		POther	Fore_FootL
		PMagnet	Fore_ToeL
		LifeSpan	2
	End

	Event
		EName	FireToeL
		Type	Local 
		At	Fore_ToeL
		
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidStreakSM.part
#		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcid.part
#		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidDark.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidBubbles.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidBubblesdark.part
		Part	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBAcidBubble.part
		Sound Firehand_loop 100.0 100.0 .55
		POther	Fore_FootL
		Lifespan	180
	End
End

Condition
	On 	Time
	Time 	11

	Event
		EName	FireFootL_Burst
		Type	Local 
		At	Fore_ToeL
		
#		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_PSlashBurst.part
		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_PSlashBurst01.part
#		POther	ClawLOffSet
		LifeSpan	9
	End
End

End