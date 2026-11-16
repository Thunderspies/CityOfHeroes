#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo

#############################################################

Input
	InpName Hips
End

#############################################################



Condition
	On	Cycle
	Time	4
	Random	1

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
			StartJitter	25.0 0.0 25.0
			Scale		2 2 2
		End
		ChildFX	:Child_Bubbles.fx
		Lifespan 30
	End

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
			StartJitter	25.0 0.0 25.0
			Scale		2 2 2
		End
		ChildFX	:Child_Bubbles_Small.fx
		Lifespan 50
	End
End

#############################################################

End