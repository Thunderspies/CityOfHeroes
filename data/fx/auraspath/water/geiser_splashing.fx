#############################################################
## Fountain_Praetorian.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	SplashAnchor1
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 0 0
		End
	End
End

Condition
	On	Cycle
	Time	1

	Event
		Type	Local
		At	SplashAnchor1
		BhvrOverride
			PositionOffset	0 0 5
		End
		ChildFX	:Child_Splashing.fx
		Lifespan 10
	End
End

#############################################################

End