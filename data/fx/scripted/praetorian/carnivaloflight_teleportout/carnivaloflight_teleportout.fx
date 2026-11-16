#############################################################
## Summon_Portal_Out.fx
#############################################################

FxInfo

## SIGIL #######################################################

Condition
	On	Time
	Time	40

	Event
		Type	Start
		At	Root
		Sound 	Seers_SummonReinforcements_Hit 50 50 1
		part	:Carnival_out_Circle_Shrinking.part
		Lifespan 30
		LifespanJitter 10
	End
End




Condition
	On	Time
	Time	35

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 7.0 0.0
		End
		Part	:Carnival_out_Circle_Rising.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	35

	Event
		Type	Local
		At	RooT
		BhvrOverride
			PositionOffset	0.0 8 0.0
		End
		Part	:Carnival_out_Beams_Vertical.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	Root
		Bhvr	behaviors\fadeOut.bhvr
		BhvrOverride
			FadeOutLength		40
		End

		Flags	AdoptParentEntity
		Lifespan 40
	End

	Event
		Type	Local
		At	Root
		ChildFX	:Carnival_out_BodyGlow.fx
		Lifespan 60
	End
End

#############################################################




End