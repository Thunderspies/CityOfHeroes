#############################################################
## BuildUp_Attack.fx
#############################################################

FxInfo

Lifespan 75

## BUILD-UP GENERIC EFFECT ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type 	local
		At	Root
		ChildFX	POWERS\Child_BuildUp.fx
	End
End

#############################################################

End