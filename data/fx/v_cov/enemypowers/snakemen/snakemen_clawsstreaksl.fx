#########################################################
##	Fireweapon
########################################################
FxInfo

LifeSpan	30

Flags	InheritAnimScale

Condition
	On 	Time
	Time 	10

	Event
		EName	Streak
		Type	Local
		At	HandL


		Part1	:SnakeMen_ClawStreaks.part
		Sound KnifeWhoosh2 30.0 30.0 .45
		Lifespan 12
	End

End


End
##################################
