#############################################################
## Fireflies_Body.fx
#############################################################

FxInfo

#########################################################


Condition
	On	Cycle
	Time	15


	Event
		Ename	GEO_Leaves_L
		Type	StartPositOnly
		At	HandL
		ChildFX	:CHILD_PARTICLE_Leaves_Hand.fx

		Lifespan	80
	End
End

Condition
	On	Cycle
	Time	35


	Event
		Ename	GEO_Leaves_L
		Type	StartPositOnly
		At	WepL
		ChildFX	:CHILD_GEO_Leaves_Head.fx
		Lifespan	80
	End
End

#############################################################

End