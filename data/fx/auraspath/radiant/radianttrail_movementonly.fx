#############################################################
## RadiantTrail.fx
#############################################################

FxInfo
Flags	DontSuppress
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
		CHILDFX	:RadiantTrail_CHILD_NoFly.fx
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
		CHILDFX	:RadiantTrail_CHILD_NoFly.fx
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
		CHILDFX	:RadiantTrail_CHILD_NoFly.fx
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
		CHILDFX	:RadiantTrail_CHILD_NoFly.fx
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
		CHILDFX	:RadiantTrail_Fly_CHILD.fx
	End
End


#Condition
#	On		TRIGGERBITS
#	TRIGGERBITS	AIR
#	DoMany		1
#

#	Event
#		Ename	go2
#		Type	Local
#		At	Root
#		WHILE	AIR
#		Flags	OneAtATime
#		CHILDFX	:StreakTrail_FLY_CHILD.fx
#	End
#End
#########################################################
End