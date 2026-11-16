#############################################################
## RadiantTrail.fx
#############################################################

FxInfo
Flags	DontSuppress
#########################################################

Condition
	On		TRIGGERBITS
	TRIGGERBITS	MALE
	DoMany		1

	Event
		Ename	go
		Type	Local
		At	Root
		UNTIL	FLY
		Flags	OneAtATime
		CHILDFX	:RadiantTrail_CHILD.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	FEMALE
	DoMany		1

	Event
		Ename	go
		Type	Local
		At	Root
		UNTIL	FLY
		Flags	OneAtATime
		CHILDFX	:RadiantTrail_CHILD.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	HUGE
	DoMany		1

	Event
		Ename	go
		Type	Local
		At	Root
		UNTIL	FLY
		Flags	OneAtATime
		CHILDFX	:RadiantTrail_CHILD.fx
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
		CHILDFX	:RadiantTrail_Fly_GRANDCHILD.fx
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