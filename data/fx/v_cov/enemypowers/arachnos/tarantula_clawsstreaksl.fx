#########################################################
##	Fireweapon
########################################################
FxInfo

Flags	InheritAnimScale

LifeSpan	30

Condition
	On 	Time
	Time 	10

	Event
		EName	Streak
		Type	Local
		At	Fore_ToeL
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakL1.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_Streaks.part
		Sound KnifeWhoosh2 30.0 30.0 .45
		Lifespan 4
	End

		Event
		EName	Streak
		Type	Local
		At	Fore_FootL
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakL1.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_Streaks.part
		#Sound	KnifeWhoosh2 30.0 15.0 .45
		Lifespan 4
	End
	
	Event
		EName	Streak
		Type	Local
		At	Fore_LLegL
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PierceStreakL1.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_Streaks.part
		#Sound	KnifeWhoosh2 30.0 15.0 .45
		Lifespan 4
	End

End



End
##################################
