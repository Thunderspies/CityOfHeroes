#############################################################
## Sprint02Glows.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest

		part3	:SprintGlows.part

	End

	Event
		Type	Local
		At	LlegL

		part3	:SprintGlowsLegs.part

	End

	Event
		Type	Local
		At	LlegR

		part3	:SprintGlowsLegs.part

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