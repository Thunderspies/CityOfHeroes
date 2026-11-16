#############################################################
## N_Fire_Rune.fx
#############################################################

FxInfo

#############################################################


Condition
	On	Cycle
	Time	150

	Event
		Type	Local
		At	Origin
		ChildFX	:N_RuneFire_GRANDCHILD.fx
		Lifespan 90
	End
End

End