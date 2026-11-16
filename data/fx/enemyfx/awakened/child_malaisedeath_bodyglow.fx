#############################################################
## CHILD_Overload_BodyGlow.fx
#############################################################


FxInfo

#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	WepR
		part	:MalaiseDeath_BodyGlow.part
		POther 	LArmR
	End

	Event
		Type	Local
		At	WepL
		part	:MalaiseDeath_BodyGlow.part
		POther 	LArmL
	End

	Event
		Type	Local
		At	FootR
		part	:MalaiseDeath_BodyGlow.part
		POther 	LLegR
	End

	Event
		Type	Local
		At	FootL
		part	:MalaiseDeath_BodyGlow.part
		POther 	LLegL
	End
End

Condition
	On	Time
	Time	2

	Event
		Type	Local
		At	LArmR
		part	:MalaiseDeath_BodyGlow.part
		POther 	UArmR
	End


	Event
		Type	Local
		At	LArmL
		part	:MalaiseDeath_BodyGlow.part
		POther 	UArmL
	End

	Event
		Type	Local
		At	LLegR
		part	:MalaiseDeath_BodyGlow.part
		part	:MalaiseDeath_BodyGlow.part

		POther 	ULegR
	End


	Event
		Type	Local
		At	LLegL
		part	:MalaiseDeath_BodyGlow.part
		part	:MalaiseDeath_BodyGlow.part

		POther 	ULegL
	End
End

Condition
	On	Time
	Time	4

	Event
		Type	Local
		At	Hips
		part	:MalaiseDeath_BodyGlow.part
		POther 	Chest
	End
End

Condition
	On	Time
	Time	6

	Event
		Type	Local
		At	Chest
		part	:MalaiseDeath_BodyGlow_Large.part
		POther 	Head
	End
End

Condition
	On	Time
	Time	8

	Event
		Type	Local
		At	Hair
		part	:MalaiseDeath_BodyGlow.part
	End
End


#########################################################

End