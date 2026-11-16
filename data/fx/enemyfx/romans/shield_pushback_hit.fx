#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 12

#############################################################
#######################AUDIO#################################
Condition
	On	Time
	Time	0
	Random	1

	Event
		EName	Sound1
		Type	Start
		At	Chest
		Sound Shield_push_back_impacts_body_02 80 80 0.9
	End

	Event
		EName	Sound2
		Type	Start
		At	Chest
		Sound Shield_push_back_impacts_body_01 100.0 100.0 .9
	End

	Event
		EName	Sound3
		Type	Start
		At	Chest
		Sound Shield_push_back_impacts_body_03 80 80 .9
	End
End

Condition
	On	Time
	Time	2

	Event
		ENAME	CloudBurstR
		Type	Start
		At	Chest
		part1	POWERS\Flight\FlightAssaultHitCloud01.part
		Pmagnet Chest
		Lifespan 12
	End
End

Condition
	On	Time
	Time	3

	Event
		ENAME	CloudBurstR
		Type	Start
		At	Chest
		part1	POWERS\Flight\FlightAssaultHitStar01.part
		part2	POWERS\Flight\FlightAssaultHitGlow01.part
		part3	POWERS\Flight\FlightAssaultHitStar02.part
		Pmagnet Chest
		Lifespan 5
	End
End

#############################################################

End