#############################################################
## Consume_Attack.fx
#############################################################

FxInfo
Lifespan 300

#############################################################

Condition
	On 	Time
	Time	30
	Event
		Type	Local
		At	Root
		Sound 	FS_Consume_Hit_01 100 100 .35
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	Head
		Part	:Consume_Smoke.part
	End

	Event
		Type	Local
		At	Head
		Part	:Consume_Core.part

	End

	Event
		Type	Local
		At	Head
		Part	:Consume_Electricity.part
		Part	:Consume_Electricity2.part

	End

End

#############################################################

End