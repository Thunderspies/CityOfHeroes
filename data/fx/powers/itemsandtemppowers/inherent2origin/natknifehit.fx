#############################################################
## HEADER
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

LifeSpan  90

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Neck
		Sound KnifeStab5 70.0 70.0 .77
	End

	Event
		EName 	Hit
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	0 .25 0
		End
		Part2	:DartHitflashStar.part
		Part1	WEAPONFX\Pistolflash.part
		Part1	WEAPONFX\MartialArtsHit.part
	End
End

#############################################################

Condition
	On	Time
	Time	0
	RepeatJitter	1

	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkUP.fx
	End
End

Condition
	On	Time
	Time	0
	Repeat		1
	RepeatJitter	1

	Event
		EName	Sparkout
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkOUT.fx
	End
End

#############################################################

End