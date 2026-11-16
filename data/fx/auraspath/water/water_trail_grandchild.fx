#############################################################
## Fountain_Praetorian.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Cycle
	Time	1

	Event
		Type	Local
		At	root
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	:Child_Splashing.fx
		Lifespan 10
	End
End

#############################################################

End