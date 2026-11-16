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
			StartJitter	4.0 0.0 4.0
		End
		ChildFX	:Child_Bubbles.fx
		Lifespan 30
	End

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
			StartJitter	4.0 0.0 4.0
		End
		ChildFX	:Child_Bubbles_Small.fx
		Lifespan 50
	End
End

Condition
	On	Time
	Time	0

	Event

		Type	PositOnly
		At	Root
		Part	:Ripples_Big.part
		Part	:Ripples_Big_Fill.part

	End
End

#############################################################

End