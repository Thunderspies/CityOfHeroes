#############################################################
## Header
#############################################################

FxInfo

Flags InheritAnimScale

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	LarmL
		BhvrOverride
			Behavior
			PositionOffset	-1.6 -.05 -.35
		End
	End

	Event
		Type	Local
		At	LarmR
		BhvrOverride
			Behavior
			PositionOffset	1.6 -.05 -.35
		End
	End

	Event
		Type	Local
		At	C_Head
		BhvrOverride
			Behavior
			PositionOffset	0 -.25 -2.2
		End
	End
End

#############################################################

End