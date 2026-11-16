#############################################################
## Consume_Attack.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Windup1
		Type	Local
		At	Head

		Part	:Consume_Smoke.part

		PMagnet	Head
		Lifespan 70
	End

	Event
		EName	Windup1
		Type	Local
		At	T_Head

		Part	:Consume_Core.part

		PMagnet	Head
		Lifespan 75
	End

	Event
		EName	Windup1
		Type	Local
		At	T_Head
		Part	:Consume_Electricity.part
		Part	:Consaume_Electricity2.part


		PMagnet	Head
		Lifespan 82
	End

End

#############################################################

End