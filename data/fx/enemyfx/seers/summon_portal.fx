#############################################################
## CostumeChange_Tech.fx
#############################################################

FxInfo
LifeSpan 120

## SIGIL #######################################################

Condition
	On	Time
	Time	0

	Event
		Type	Start
		At	Root
		Sound Seers_SummonReinforcements_Hit 50 50 1
		part	:GroundCircle_Shrinking.part
		Lifespan 80
		LifespanJitter 10
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Start
		At	Root
		part	:GroundCircle_Still.part
		Lifespan 90
		LifespanJitter 10
	End
End


Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	Root
		Part	:GroundCircle_Rising.part
		Lifespan 60
	End

	Event
		Type	Local
		At	RooT
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
		End
		Part	:Beams_Vertical.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	25

	Event
		Type	Local
		At	Root
		ChildFX	:BodyGlow.fx
		Lifespan 60
	End
End

#############################################################




End