#############################################################
## Fire_Trail_MovementOnly.fx
#############################################################

FxInfo

#############################################################

Condition
	On		TRIGGERBITS
	TRIGGERBITS	MALE
	Domany 1

	Event
		Ename	MaleAura
		Type	Local
		At	Root
		Flags	OneAtATime
		WHILE	MALE
		ChildFX	:Fire_Impact_CHILD_NOAIR.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	FEMALE
	Domany 1

	Event
		Ename	FemaleAura
		Type	Local
		At	Root
		Flags	OneAtATime
		WHILE	FEMALE
		ChildFX	:Fire_Impact_CHILD_NOAIR.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	HUGE
	Domany 1

	Event
		Ename	HugeAura
		Type	Local
		At	Root
		Flags	OneAtATime
		WHILE	HUGE
		ChildFX	:Fire_Impact_CHILD_NOAIR.fx
	End
End

#############################################################

Condition

	On 		TRIGGERBITS
	TRIGGERBITS     FORWARD
	DoMany		1

	Event
		ENAME	TrailGround
		Type	Local
		At	Root
		WHILE 	FORWARD
		Flags	OneAtATime
		ChildFX	:Fire_Trail_NOAIR_MovementOnly.fx
	End
End

Condition

	On 		TRIGGERBITS
	TRIGGERBITS     BACKWARD
	DoMany		1

	Event
		ENAME	TrailGround
		Type	Local
		At	Root
		WHILE 	BACKWARD
		Flags	OneAtATime
		ChildFX	:Fire_Trail_NOAIR_MovementOnly.fx
	End
End

Condition

	On 		TRIGGERBITS
	TRIGGERBITS     STEPLEFT
	DoMany		1

	Event
		ENAME	TrailGround
		Type	Local
		At	Root
		WHILE 	STEPLEFT
		Flags	OneAtATime
		ChildFX	:Fire_Trail_NOAIR_MovementOnly.fx
	End
End

Condition

	On 		TRIGGERBITS
	TRIGGERBITS     STEPRIGHT
	DoMany		1

	Event
		ENAME	TrailGround
		Type	Local
		At	Root
		WHILE 	STEPRIGHT
		Flags	OneAtATime
		ChildFX	:Fire_Trail_NOAIR_MovementOnly.fx
	End
End


Condition
	On		TRIGGERBITS
	TRIGGERBITS	FLY
	DoMany		1

	Event
		EName	FireIdle
		Type	Local
		At	Hips
		WHILE	FLY
		Flags	OneAtATime
		BhvrOverride
			PositionOffset 0 5 0
		End
		ChildFX	:Fire_Flying_CHILD.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	AIR
	DoMany		1

	Event
		EName	FireIdle
		Type	Local
		At	Hips
		WHILE	AIR
		Flags	OneAtATime
		ChildFX	:Fire_Air_CHILD.fx
	End
End

#########################################################

End