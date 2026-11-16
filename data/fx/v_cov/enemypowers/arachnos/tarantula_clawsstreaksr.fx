#########################################################
##	Fireweapon
########################################################
FxInfo

Flags	InheritAnimScale

LifeSpan	30

Condition
	On 	Time
	Time 	11

	Event
		EName	Streak
		Type	Local
		At	Fore_ToeR
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakR1.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_Streaks.part
		Sound KnifeWhoosh2 30.0 30.0 .45
		Lifespan 4
	End

	Event
		EName	Streak
		Type	Local
		At	Fore_FootR
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakR1.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_Streaks.part
		#Sound	KnifeWhoosh2 30.0 15.0 .45
		Lifespan 4
	End
	
	Event
		EName	Streak
		Type	Local
		At	Fore_LLegR
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakR1.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_Streaks.part
		#Sound	KnifeWhoosh2 30.0 15.0 .45
		Lifespan 4
	End
End


End
##################################
