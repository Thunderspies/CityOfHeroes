#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 40

#############################################################

Condition
	On 	Time
	Time	1
	Event
		Type	Local
		At	Root
		Sound 	Brawling_SweepingCross_Attack_01 200 100 .8
	End
End

Condition
	On	Time
	Time	11
	Event
		Type	Local
		At	WepR
		Part	:Hand_Buildup_Star.part
		Part	:Hand_Buildup_Ring.part

	End
End

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	WepR
		Part	:Hand_Trail.part
		Part	:Hand_Trail_Additive.part
		Lifespan 20
	End
End

Condition
	On		TriggerBits
	TriggerBits	MALE
	DoMany		1

	Event
		EName	Swoosh
		Type	Start
		At	Root
		Flags	OneAtATime
		UNTIL	FLY
		ChildFX	:CHILD_SweepingCross_Attack.fx
		Lifespan 30
	End
End

Condition
	On		TriggerBits
	TriggerBits	FEMALE
	DoMany		1

	Event
		EName	Swoosh
		Type	Start
		At	Root
		Flags	OneAtATime
		UNTIL	FLY
		ChildFX	:CHILD_SweepingCross_Attack.fx
		Lifespan 30
	End
End

Condition
	On		TriggerBits
	TriggerBits	HUGE
	DoMany		1

	Event
		EName	Swoosh
		Type	Start
		At	Root
		Flags	OneAtATime
		UNTIL	FLY
		ChildFX	:CHILD_SweepingCross_Attack.fx
		Lifespan 30
	End
End

Condition
	On		TriggerBits
	TriggerBits	FLY
	DoMany		1

	Event
		EName	SwooshAIR
		Type	Start
		At	Root
		Flags	OneAtATime
		WHILE	FLY
		ChildFX	:CHILD_AIR_SweepingCross_Attack.fx
		Lifespan 30
	End
End
#############################################################

End