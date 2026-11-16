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
		Part	:Interface_BodyGlow.part
		POther	LLegL
	End

	Event
		Type	Local
		At	FootR
		Part	:Interface_BodyGlow.part
		POther	LLegR
	End

	Event
		Type	Local
		At	FootL
		Part	:Interface_BodyGlow.part
	End

	Event
		Type	Local
		At	FootR
		Part	:Interface_BodyGlow.part
	End
End

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	LLegL
		Part	:Interface_BodyGlow.part
		POther	ULegL
	End

	Event
		Type	Local
		At	LLegR
		Part	:Interface_BodyGlow.part
		POther	ULegR
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Hips
		Part	:Interface_BodyGlow.part
		POther	Chest
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	UArmL
		Part	:Interface_BodyGlow.part
		POther	LArmL
	End

	Event
		Type	Local
		At	UArmR
		Part	:Interface_BodyGlow.part
		POther	LArmR
	End

	Event
		Type	Local
		At	Chest
		Part	:Interface_BodyGlow.part
		POther	Neck
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	LArmL
		Part	:Interface_BodyGlow.part
		POther	WepL
	End

	Event
		Type	Local
		At	LArmR
		Part	:Interface_BodyGlow.part
		POther	WepR
	End

	Event
		Type	Local
		At	Neck
		Part	:Interface_BodyGlow.part
		POther	Hair
	End
End
#############################################################

End