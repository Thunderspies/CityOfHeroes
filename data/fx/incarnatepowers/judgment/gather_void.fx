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
		At	Hips
		BhvrOverride
          		PositionOffset 1.0 0 1.0

         	End
		Part2	:GatherDust.part
		Part2	:GatherDust_VoidBlend.part
		Part2	:GatherDust_Voidadd.part
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
		At	Hips
		Part2	:GatherDust_VoidHoleAdd.part
#		Part2	:GatherDust_VoidHoleBlend.part
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
          		PositionOffset 1.0 0.5 1.0

         	End
		Part2	:GatherDust_VoidFloorBlend.part
		Part2	:GatherDust_VoidFloorAdd.part
		Lifespan	10
	End

End
#Condition
#	On	Time
#	Time	4
#
#	Event
#		EName	AOERing
#		Type	start
#		At	Root
#		BhvrOverride
#          		PositionOffset 0 1.5 0
#
#         	End
#
##		Part2	:GatherDust_Voidglow.part
#		Lifespan	20
#	End
#
#End
Condition
	On	Time
	Time	42

	Event
		EName	AOERing
		Type	start
		At	Root
		BhvrOverride
          		PositionOffset 0 0.25 0

         	End

		Part2	:Gather_Void_Area.part
		Part2	:Gather_Void_AreaFloor.part
		Part2	:Gather_Void_AreaFloorB.part
		Lifespan	30
	End

End
Condition
	On	Time
	Time	20

	Event
		EName	AOERing
		Type	start
		At	Hips
		BhvrOverride
          		PositionOffset 0 1.4 0

         	End

		Part2	:Gather_Void_Tentacles.part
		Part2	:Gather_Void_TentaclesB.part
		Part2	:Gather_Void_TentaclesPieces.part
		Part2	:Gather_Void_AreaFloorBlast.part
#		Part2	:Gather_Void_TentaclesC.part
		Lifespan	100
	End

End
Condition
	On	Time
	Time	45

	Event
		EName	AOERing
		Type	start
		At	root
		BhvrOverride
          		PositionOffset 10 1.5 0

         	End

		Part2	:Gather_Void_TentaclesArea.part


		Lifespan	100
	End

End
Condition
	On	Time
	Time	45

	Event
		EName	AOERing
		Type	start
		At	root
		BhvrOverride
          		PositionOffset -10 1.5 0

         	End

		Part2	:Gather_Void_TentaclesArea.part


		Lifespan	100
	End

End
Condition
	On	Time
	Time	45

	Event
		EName	AOERing
		Type	start
		At	root
		BhvrOverride
          		PositionOffset 0 1.5 10

         	End

		Part2	:Gather_Void_TentaclesArea.part


		Lifespan	100
	End

End
Condition
	On	Time
	Time	45

	Event
		EName	AOERing
		Type	start
		At	root
		BhvrOverride
          		PositionOffset 0 1.5 -10

         	End

		Part2	:Gather_Void_TentaclesArea.part


		Lifespan	100
	End

End
End