#############################################################
## InvisStealth.fx
#############################################################

FxInfo

LifeSpan 60

#############################################################

Condition
	On	Time
	Time	0


	Event
		Type	Local
		At 	Origin
		Sound rumble1 80 80 0.8
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At 	Origin
		Sound invisible1 80 80 0.5
	End
End

#############################################################

Condition
	On	Time
	Time	10

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	POWERS\Invisibility\InvisBodyGlow02.part
		POther	LarmR
		Lifespan 0

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	POWERS\Invisibility\InvisBodyGlow02.part
		POther	LarmL
		Lifespan 0
	End
End

Condition
	On	Time
	Time	20

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	POWERS\Invisibility\InvisBodyGlow02.part
		POther	LarmR
		Lifespan 0
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	POWERS\Invisibility\InvisBodyGlow02.part
		POther	LarmL
		Lifespan 0
	End
End

Condition
	On	Time
	Time	25

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	POWERS\Invisibility\InvisBodyGlow02.part
		POther	Neck
		Lifespan 0
	End
End

Condition
	On	Time
	Time	30

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	POWERS\Invisibility\InvisBodyGlow02.part
		POther	Chest
		Lifespan 0
	End
End

Condition
	On	Time
	Time	35

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part	POWERS\Invisibility\InvisBodyGlow02.part
		POther	Ulegl
		Lifespan 0
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part	POWERS\Invisibility\InvisBodyGlow02.part
		POther	UlegR
		Lifespan 0
	End
End

Condition
	On	Time
	Time	40

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	POWERS\Invisibility\InvisBodyGlow02.part
		POther	Llegl
		Lifespan 0
	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	POWERS\Invisibility\InvisBodyGlow02.part
		POther	LlegR
		Lifespan 0
	End
End

#############################################################

End