#############################################################
## StreakTrail.fx
#############################################################

FxInfo
Flags	DontSuppress

#########################################################


Condition
	On	Time
	Time	0

	Event
		Ename	IdleParts
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 0 -0.65
		End
		Part	:StreakTrail_Idle_Ground.part
		Part	:StreakTrail_Idle.part
		Part	:StreakTrail_Idle_Rings.part
		Part	:StreakTrail_Idle_Rings2.part
	End
End

#########################################################

Condition
	On		TRIGGERBITS
	TRIGGERBITS	FORWARD
	DoMany		1

	Event
		Ename	go
		Type	Local
		At	Root
		WHILE	FORWARD
		Flags	OneAtATime
		CHILDFX	:StreakTrail_NOFLY_MovementOnly.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	BACKWARD
	DoMany		1

	Event
		Ename	go
		Type	Local
		At	Root
		WHILE	BACKWARD
		Flags	OneAtATime
		CHILDFX	:StreakTrail_NOFLY_MovementOnly.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	STEPLEFT
	DoMany		1

	Event
		Ename	go
		Type	Local
		At	Root
		WHILE	STEPLEFT
		Flags	OneAtATime
		CHILDFX	:StreakTrail_NOFLY_MovementOnly.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	STEPRIGHT
	DoMany		1

	Event
		Ename	go
		Type	Local
		At	Root
		WHILE	STEPRIGHT
		Flags	OneAtATime
		CHILDFX	:StreakTrail_NOFLY_MovementOnly.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	FLY
	DoMany		1

	Event
		Ename	go2
		Type	Local
		At	Root
		WHILE	FLY
		Flags	OneAtATime
		CHILDFX	:StreakTrail_FLY_CHILD.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	AIR
	DoMany		1

	Event
		Ename	go2
		Type	Local
		At	Root
		WHILE	AIR
		Flags	OneAtATime
		CHILDFX	:StreakTrail_FLY_CHILD.fx
	End
End



#########################################################

End