#########################################################
##	Fireweapon
########################################################
FxInfo

Flags	InheritAnimScale
Lifespan 60

#################################################################################
##################################  Windup  #####################################
#################################################################################
#################################################################################
#################################  R I G H T  ###################################
#################################################################################

Condition
	On 	Time
	Time 	5

	Event
		EName	Streak
		Type	Local
		At	Fore_ToeR
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakR1.bhvr
#		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_StreaksWindUp.part
		Lifespan 11
	End

	Event
		EName	Streak
		Type	Local
		At	Fore_FootR
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakR1.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_StreaksWindUp.part
		Lifespan 11
	End
	
	Event
		EName	Streak
		Type	Local
		At	Fore_LLegR
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakR1.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_StreaksWindUp.part
		Lifespan 11
	End
End


#################################################################################
##################################  L E F T  ####################################
#################################################################################

Condition
	On 	Time
	Time 	5

	Event
		EName	Streak
		Type	Local
		At	Fore_ToeL
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakL1.bhvr
#		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_StreaksWindUp.part
		Lifespan 11
	End

		Event
		EName	Streak
		Type	Local
		At	Fore_FootL
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakL1.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_StreaksWindUp.part
		Lifespan 11
	End
	
	Event
		EName	Streak
		Type	Local
		At	Fore_LLegL
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakL1.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_StreaksWindUp.part
		Lifespan 11
	End

End



#################################################################################
##################################  Right Alt  ###################################
##################################################################################

Condition
	On 	Time
	Time 	4

	Event
		EName	Streak
		Type	Local
		At	HandR

		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakR2.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_StreaksWindUp.part
		Lifespan 11
	End

	Event
		EName	Streak
		Type	Local
		At	LArmR
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakR2.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_StreaksWindUp.part
		Lifespan 11
	End
	
	Event
		EName	Streak
		Type	Local
		At	UArmR
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakR2.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_StreaksWindUp.part
		Lifespan 11
	End

End


#################################################################################
##################################  Left Alt  ###################################
#################################################################################

Condition
	On 	Time
	Time 	4

	Event
		EName	Streak
		Type	Local
		At	HandL

		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakL2.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_StreaksWindUp.part
		Lifespan 11
	End

		Event
		EName	Streak
		Type	Local
		At	LArmL
		

		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakL2.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_StreaksWindUp.part
		Lifespan 11
	End
	
	Event 
		EName	Streak
		Type	Local
		At	UArmL
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakL2.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_StreaksWindUp.part
		Lifespan 1
	End

End


#################################################################################
################################  S W I N G  ####################################
#################################################################################
#################################################################################
#################################  R I G H T  ###################################
#################################################################################

Condition
	On 	Time
	Time 	14

	Event
		EName	Streak
		Type	Local
		At	Fore_ToeR
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakR1.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_Streaks.part
		Sound KnifeWhoosh2 30.0 30.0 .45
		Lifespan 7
	End

	Event
		EName	Streak
		Type	Local
		At	Fore_FootR
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakR1.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_Streaks.part
		Lifespan 7
	End
	
	Event
		EName	Streak
		Type	Local
		At	Fore_LLegR
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakR1.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_Streaks.part
		Lifespan 7
	End
End


#################################################################################
##################################  L E F T  ####################################
#################################################################################

Condition
	On 	Time
	Time 	14

	Event
		EName	Streak
		Type	Local
		At	Fore_ToeL
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakL1.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_Streaks.part
		Lifespan 7
	End

		Event
		EName	Streak
		Type	Local
		At	Fore_FootL
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakL1.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_Streaks.part
		Lifespan 7
	End
	
	Event
		EName	Streak
		Type	Local
		At	Fore_LLegL
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakL1.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_Streaks.part
		Lifespan 7
	End

End



#################################################################################
#################################  Right Alt  ###################################
#################################################################################

Condition
	On 	Time
	Time 	14

	Event
		EName	Streak
		Type	Local
		At	HandR

		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakR2.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_Streaks.part
		Lifespan 4
	End

	Event
		EName	Streak
		Type	Local
		At	LArmR
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakR2.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_Streaks.part
		Lifespan 4
	End
	
	Event
		EName	Streak
		Type	Local
		At	UArmR
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakR2.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_Streaks.part
		Lifespan 4
	End

End


#################################################################################
##################################  Left Alt  ###################################
#################################################################################

Condition
	On 	Time
	Time 	14

	Event
		EName	Streak
		Type	Local
		At	HandL

		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakL2.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_Streaks.part
		Lifespan 4
	End

		Event
		EName	Streak
		Type	Local
		At	LArmL
		

		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakL2.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_Streaks.part
		Lifespan 4
	End
	
	Event 
		EName	Streak
		Type	Local
		At	UArmL
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakL2.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_Streaks.part
		Lifespan 4
	End

End


End
##################################
