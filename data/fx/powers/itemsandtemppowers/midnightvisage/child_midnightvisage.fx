#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 100

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		part	:Smoke01.part
		part	:Smoke02.part
		part	:Smoke_Wispy01.part
		part	:Smoke_Wispy02.part
		Lifespan 3
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		part	:Sparkles01.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset		0.0 7.0 0.0
		End
		part	:Moon01.part
		Lifespan 60
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Neck
		part	:Body_Smoke01.part
		POther	Hips
		Lifespan 2
	End

	Event
		Type	Local
		At	Hair
		part	:Body_Smoke01.part
		POther	Neck
		Lifespan 2
	End

	Event
		Type	Local
		At	ULegL
		part	:Body_Smoke01.part
		POther	LLegL
		Lifespan 2
	End

	Event
		Type	Local
		At	ULegR
		part	:Body_Smoke01.part
		POther	LLegR
		Lifespan 2
	End

	Event
		Type	Local
		At	UArmL
		part	:Body_Smoke01.part
		POther	LArmL
		Lifespan 2
	End

	Event
		Type	Local
		At	UArmR
		part	:Body_Smoke01.part
		POther	LArmR
		Lifespan 2
	End
End

Condition
	On	Time
	Time	1

	Event
		Type	Local
		At	LLegL
		part	:Body_Smoke01.part
		POther	FootL
		Lifespan 2
	End

	Event
		Type	Local
		At	LLegR
		part	:Body_Smoke01.part
		POther	FootR
		Lifespan 2
	End

	Event
		Type	Local
		At	UArmL
		part	:Body_Smoke01.part
		POther	WepL
		Lifespan 2
	End

	Event
		Type	Local
		At	LArmR
		part	:Body_Smoke01.part
		POther	WepR
		Lifespan 2
	End
End

#############################################################

End