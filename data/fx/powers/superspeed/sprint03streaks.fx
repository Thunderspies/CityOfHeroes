#############################################################
## Sprint03Streaks.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	eyes
		part3	:SprintStreaksLegs.part
	End

	Event
		Type	Local
		At	ULegL
		part3	:SprintStreaksLegs.part
	End

	Event
		Type	Local
		At	ULegR
		part3	:SprintStreaksLegs.part
	End

	Event
		Type	Local
		At	Chest
		part3	:SprintStreaksLegs.part
	End

	Event
		Type	Local
		At	LlegL
		part3	:SprintStreaksLegs.part
	End

	Event
		Type	Local
		At	LlegR
		part3	:SprintStreaksLegs.part
	End

	Event
		Type	Local
		At	UarmL
		part3	:SprintStreaksLegs.part
	End

	Event
		Type	Local
		At	UArmR
		part3	:SprintStreaksLegs.part
	End
End

#############################################################
## Ground & Physics FX
#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		ChildFX	fx\POWERS\SuperSpeed\Sprint.fx
	End
End

#############################################################

End