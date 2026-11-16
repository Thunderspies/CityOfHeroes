#########################################################
## Vanguard "Redding" Rifle - Sigil Round (Hit)
#########################################################

FxInfo

LifeSpan 40

## SIGIL #######################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	FootL
		Part	:BodyGlow.part
		POther	LLegL
	End

	Event
		Type	Local
		At	FootR
		Part	:BodyGlow.part
		POther	LLegR
	End

	Event
		Type	Local
		At	FootL
		Part	:BodyGlow.part
	End

	Event
		Type	Local
		At	FootR
		Part	:BodyGlow.part
	End
End

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	LLegL
		Part	:BodyGlow.part
		POther	ULegL
	End

	Event
		Type	Local
		At	LLegR
		Part	:BodyGlow.part
		POther	ULegR
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Hips
		Part	:BodyGlow.part
		POther	Chest
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	UArmL
		Part	:BodyGlow.part
		POther	LArmL
	End

	Event
		Type	Local
		At	UArmR
		Part	:BodyGlow.part
		POther	LArmR
	End

	Event
		Type	Local
		At	Chest
		Part	:BodyGlow.part
		POther	Neck
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	LArmL
		Part	:BodyGlow.part
		POther	WepL
	End

	Event
		Type	Local
		At	LArmR
		Part	:BodyGlow.part
		POther	WepR
	End

	Event
		Type	Local
		At	Neck
		Part	:BodyGlow.part
		POther	Hair
	End
End
#############################################################

End