#########################################################
##
#########################################################

FxInfo

LifeSpan 40

## SIGIL #######################################################

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	FootL
		Part	:Carnival_Out_BodyGlow.part
		POther	LLegL
	End

	Event
		Type	Local
		At	FootR
		Part	:Carnival_Out_BodyGlow.part
		POther	LLegR
	End

	Event
		Type	Local
		At	FootL
		Part	:Carnival_Out_BodyGlow.part
	End

	Event
		Type	Local
		At	FootR
		Part	:Carnival_Out_BodyGlow.part
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	LLegL
		Part	:Carnival_Out_BodyGlow.part
		POther	ULegL
	End

	Event
		Type	Local
		At	LLegR
		Part	:Carnival_Out_BodyGlow.part
		POther	ULegR
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Hips
		Part	:Carnival_Out_BodyGlow.part
		POther	Chest
	End
End

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	UArmL
		Part	:Carnival_Out_BodyGlow.part
		POther	LArmL
	End

	Event
		Type	Local
		At	UArmR
		Part	:Carnival_Out_BodyGlow.part
		POther	LArmR
	End

	Event
		Type	Local
		At	Chest
		Part	:Carnival_Out_BodyGlow.part
		POther	Neck
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	LArmL
		Part	:Carnival_Out_BodyGlow.part
		POther	WepL
	End

	Event
		Type	Local
		At	LArmR
		Part	:Carnival_Out_BodyGlow.part
		POther	WepR
	End

	Event
		Type	Local
		At	Neck
		Part	:Carnival_Out_BodyGlow.part
		POther	Hair
	End
End
#############################################################

End