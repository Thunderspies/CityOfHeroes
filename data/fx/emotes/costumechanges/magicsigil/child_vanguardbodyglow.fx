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
		Part	:VanguardBodyGlow01.part
		POther	LLegL
	End

	Event
		Type	Local
		At	FootR
		Part	:VanguardBodyGlow01.part
		POther	LLegR
	End
End

Condition
	On	Time
	Time	7

	Event
		Type	Local
		At	LLegL
		Part	:VanguardBodyGlow01.part
		POther	ULegL
	End

	Event
		Type	Local
		At	LLegR
		Part	:VanguardBodyGlow01.part
		POther	ULegR
	End
End

Condition
	On	Time
	Time	14

	Event
		Type	Local
		At	Hips
		Part	:VanguardBodyGlow01.part
		POther	Chest
	End
End

Condition
	On	Time
	Time	21

	Event
		Type	Local
		At	UArmL
		Part	:VanguardBodyGlow01.part
		POther	LArmL
	End

	Event
		Type	Local
		At	UArmR
		Part	:VanguardBodyGlow01.part
		POther	LArmR
	End

	Event
		Type	Local
		At	Chest
		Part	:VanguardBodyGlow01.part
		POther	Neck
	End
End

Condition
	On	Time
	Time	28

	Event
		Type	Local
		At	LArmL
		Part	:VanguardBodyGlow01.part
		POther	WepL
	End

	Event
		Type	Local
		At	LArmR
		Part	:VanguardBodyGlow01.part
		POther	WepR
	End

	Event
		Type	Local
		At	Neck
		Part	:VanguardBodyGlow01.part
		POther	Hair
	End
End
#############################################################

End