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

Condition
	On	Time
	Time	0

	Event
		Type	start
		At	Root
		Sound explohit 80 80 0.77
	End
End

## POWER FX #######################################################


Condition
	On	Time
	Time	1

	Event
		EName	AOERing
		Type	start
		At	Root
		BhvrOverride
          		PositionOffset 1.0 1.5 1.0

         	End
		Part2	:GatherDust.part
		Part2	:GatherDustBigFloor.part
		Lifespan	10
	End

End
Condition
	On	Time
	Time	1

	Event
		EName	AOERing
		Type	start
		At	Root
		BhvrOverride
          		PositionOffset 0 4 0

         	End
		Part2	:GatherDust_Fire.part
		Lifespan	10
	End

End
Condition
	On	Time
	Time	1

	Event
		EName	AOERing
		Type	start
		At	Root
		BhvrOverride
          		PositionOffset 1.0 1 1.0

         	End
		Part2	:GatherDust_Fireb.part
		Lifespan	10
	End

End
Condition
	On	Time
	Time	15

	Event
		EName	AOERing
		Type	start
		At	Root
		BhvrOverride
       		PositionOffset 0 -1 0

         	End
		Part2	:Gather_Fire_Area.part
		Lifespan	1
	End

End
Condition
	On	Time
	Time	15

	Event
		EName	AOERing
		Type	start
		At	Root
		BhvrOverride
       		PositionOffset 0 1 0

         	End
		Part2	:Gather_Fire_AreaFloor.part
		Lifespan	1
	End

End
Condition
	On	Time
	Time	20

	Event
		EName	AOERing
		Type	start
		At	Root
		BhvrOverride
       		PositionOffset 0 0.2 0

         	End
		Part2	:Gather_Fire_AreaSmoke.part
		Part2	:Gather_Fire_AreaBurnedEarth.part
		Lifespan	1
	End

End
##########################################################

End