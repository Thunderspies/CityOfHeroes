#############################################################
## CHILD_HeavyBlow_Left.fx
#############################################################

FxInfo
Lifespan 32

#############################################################

Condition
	On		TriggerBits
	TriggerBits	MALE
	DoMany		1

	Event
		EName	Swoosh
		Type	Local
		At	Origin
		Flags	OneAtATime
		UNTIL	FLY
		ChildFX	:CHILD2_AIR_HeavyBlow_Left_Attack.fx
		Lifespan 30
	End
End

Condition
	On		TriggerBits
	TriggerBits	FEMALE
	DoMany		1

	Event
		EName	Swoosh
		Type	Local
		At	Origin
		Flags	OneAtATime
		UNTIL	FLY
		ChildFX	:CHILD2_AIR_HeavyBlow_Left_Attack.fx
		Lifespan 30
	End
End

Condition
	On		TriggerBits
	TriggerBits	HUGE
	DoMany		1

	Event
		EName	Swoosh
		Type	Local
		At	Root
		Flags	OneAtATime
		UNTIL	FLY
		ChildFX	:CHILD2_AIR_HeavyBlow_Left_Attack.fx
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
		ChildFX	:CHILD2_AIR_HeavyBlow_Left_Attack.fx
		Lifespan 30
	End
End

#############################################################

End