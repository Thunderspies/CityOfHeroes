#############################################################
## Paralytic.fx
#############################################################

FxInfo

Lifespan 200

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Chest
		part	:FlakesOut_Cust.part
		Lifespan 20
	End
		Event
		Type	Local
		At	Root
		Sound 	Burnout_01 100 100 .8
	End


End

Condition
	On	Time
	Time	25

	Event
		Type	Local
		At	Chest
		part	:Flash_Cust.part
		Lifespan 8
	End

End

Condition
	On	Time
	Time	45

	Event
		Type	Local
		At	Chest
		part	:FlashIn_Cust.part
		Lifespan 3
	End
End

Condition
	On	Time
	Time	50

	Event
		Type	Local
		At	Chest
		part	:Glow_Cust.part
		part2	:GoldenCenter_Cust.part
		part3	:Lingering_Cloud_Cust.part
		part	:FlakesIn_Cust.part
		part	:Swirl_01_Cust.part
		Lifespan 55
	End
End

#############################################################


End