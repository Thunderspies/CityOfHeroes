#############################################################
## Header
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On	Cycle
	Time	10
	Chance	0.30

	Event
		ENAME	Head
		Type	Local
		At	Hair
		part	:BodySparkle01.part
		POther	Neck
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	10
	Chance	0.30

	Event
		Type	Local
		At	Neck
		part	:BodySparkle01.part
		POther  Chest
		Lifespan 5
	End

	Event
		Type	Local
		At	Hips
		part	:BodySparkle01.part
		POther  UArmL
		Lifespan 5
	End

	Event
		Type	Local
		At	Hips
		part	:BodySparkle01.part
		POther  UArmR
		Lifespan 5
	End

	Event
		Type	Local
		At	ULegR
		part	:BodySparkle01.part
		POther	UArmR
		Lifespan 5
	End

	Event
		Type	Local
		At	ULegL
		part	:BodySparkle01.part
		POther	UArmL
		Lifespan 5
	End

	Event
		Type	Local
		At	UArmR
		part	:BodySparkle01.part
		POther	UArmL
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	10
	Chance	0.30

	Event
		Type	Local
		At	ULegR
		part	:BodySparkle01.part
		POther	ULegL
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	10
	Chance	0.30

	Event
		Type	Local
		At	UArmR
		part	:BodySparkle01.part
		POther	LArmR
		Lifespan 5
	End

	Event
		Type	Local
		At	LArmR
		part	:BodySparkle01.part
		POther	WepR
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	10
	Chance	0.30

	Event
		Type	Local
		At	UArmL
		part	:BodySparkle01.part
		POther	LArmL
		Lifespan 5
	End

	Event
		Type	Local
		At	LArmL
		part	:BodySparkle01.part
		POther	WepL
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	10
	Chance	0.30

	Event
		Type	Local
		At	ULegR
		part	:BodySparkle01.part
		POther	LLegR
		Lifespan 5
	End

	Event
		Type	Local
		At	ULegL
		part	:BodySparkle01.part
		POther	LLegL
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	10
	Chance	0.30

	Event
		Type	Local
		At	LLegR
		part	:BodySparkle01.part
		POther	FootR
		Lifespan 5
	End

	Event
		Type	Local
		At	LLegL
		part	:BodySparkle01.part
		POther	FootL
		Lifespan 5
	End
End

#############################################################

End