#########################################################
## IncarnatePower  - Gather
#########################################################

FxInfo
LifeSpan 300

########################################################

Input
	Inpname	Hips
End

########################################################


## SOUND FX #######################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		Type	start
#		At	Root
#		Sound explohit 80 80 0.77
#	End
#End

## POWER FX #######################################################


Condition
	On	Time
	Time	0

	Event
		EName	AOERing
		Type	start
		At	Chest
		part1	:Incarnate_Sparkles.part
		Part2	:Incarnate_StreakRing.part
		Part3	:Incarnate_GatherDust_Glow.part
		Part4	:Incarnate_GatherDust.part
		Lifespan	14
	End
	Event
		Type	Local
		At	Root
		Sound 	Incarnate_Gather_01 100 100 .8
	End

End

End