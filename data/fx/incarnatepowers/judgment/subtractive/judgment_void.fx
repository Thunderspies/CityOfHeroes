#############################################################
## Judgment_Void.fx
#############################################################

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
	Time	12

	Event
		Type	start
		At	Root
		Sound Incarnate_VoidJudgement_Attack_01 100 100 .8
	End
End

## POWER FX #######################################################

Condition
	On	Time
	Time	0

	Event
		EName	AOERing
		Type	start
		At	Chest
		ChildFX :fx\IncarnatePowers\Incarnate_Gather.fx
		Lifespan	30
	End
End


Condition
	On	Time
	Time	12

	Event
		EName	AOERing
		Type	start
		At	Root
		BhvrOverride
          		PositionOffset 1.0 0.5 1.0

         	End
		Part2	:Judgment_Void_FloorBlend.part
		Part2	:Judgment_Void_FloorAdd.part
		Lifespan	10
	End

	Event
		EName	AOERing
		Type	Local
		At	Chest
		Part2	:Judgment_Void_Blend.part
		Part2	:Judgment_Void_add.part
		Part2	:Judgment_Void_HoleStart.part
		Lifespan	8
	End

End


Condition
	On	Time
	Time	48

	Event
		EName	AOERing
		Type	start
		At	Chest
		BhvrOverride
          		PositionOffset 0 1.4 0

         	End
		Part	:Judgment_Void_Flash.part
		Part2	:Judgment_Void_TentaclesPieces.part

		Part2	:Judgment_Void_Tentacles.part
		Part2	:Judgment_Void_TentaclesB.part

		Part2	:Judgment_Void_Tentacles_Blur.part
		Lifespan	5
	End
End



Condition
	On	Time
	Time	51

	Event
		EName	AOERing
		Type	start
		At	Chest
		BhvrOverride
          		PositionOffset 0 1.4 0

         	End
		Part2	:Judgment_Void_AreaExplosion.part
		Lifespan	3
	End
End

Condition
	On	Time
	Time	55

	Event
		EName	AOERing
		Type	start
		At	Root
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			Spin		0 -0.2 0
          		PositionOffset 0 1.25 0

         	End

		Part	:Judgment_Void_Area.part
		Part	:Judgment_Void_AreaFloor.part
		Part	:Judgment_Void_HoleFloor.part
		Part	:Judgment_Void_HoleFloorB.part

		Lifespan	45
	End

End

Condition
	On	Time
	Time	60

	Event
		EName	AOERing
		Type	start
		At	root


		Part2	:Judgment_Void_TentaclesArea.part
		PMagnet	Root

		Lifespan	30
	End

End

########################################################

End