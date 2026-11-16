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
		part	:Overload_BodyGlow.part
		POther 	LArmR
	End

	Event
		Type	Local
		At	WepL
		part	:Overload_BodyGlow.part
		POther 	LArmL
	End

	Event
		Type	Local
		At	FootR
		part	:Overload_BodyGlow.part
		POther 	LLegR
	End

	Event
		Type	Local
		At	FootL
		part	:Overload_BodyGlow.part
		POther 	LLegL
	End
End

Condition
	On	Time
	Time	2

	Event
		Type	Local
		At	LArmR
		part	:Overload_BodyGlow.part
		POther 	UArmR
	End


	Event
		Type	Local
		At	LArmL
		part	:Overload_BodyGlow.part
		POther 	UArmL
	End

	Event
		Type	Local
		At	LLegR
		part	:Overload_BodyGlow.part
		POther 	ULegR
	End


	Event
		Type	Local
		At	LLegL
		part	:Overload_BodyGlow.part
		POther 	ULegL
	End
End

Condition
	On	Time
	Time	4

	Event
		Type	Local
		At	Hips
		part	:Overload_BodyGlow.part
		POther 	Chest
	End
End

Condition
	On	Time
	Time	6

	Event
		Type	Local
		At	Chest
		part	:Overload_BodyGlow.part
		POther 	Head
	End
End

Condition
	On	Time
	Time	8

	Event
		Type	Local
		At	Head
		part	:Overload_BodyGlow.part
	End
End


#########################################################

End