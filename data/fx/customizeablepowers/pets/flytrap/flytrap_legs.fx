#############################################################
## FlyTrap_Resistance.fx
#############################################################

FxInfo

Flags InheritAlpha InheritAnimScale

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Body
		Type	Local
		At	Root
		BhvrOverride
			TintGeom		1
		End
		Anim	FlyTrap_Body
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Body
		BhvrOverride
			TintGeom		1
		End
		AnimPiv	Hips
		Anim	FlyTrap_Arms
	End
End

Condition
	Event
		Type	Local
		At	Body
		BhvrOverride
			TintGeom		1
		End
		AnimPiv	Hips
		Anim	FlyTrap_Head
	End
End

#############################################################

End