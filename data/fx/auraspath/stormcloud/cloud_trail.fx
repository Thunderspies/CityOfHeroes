#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################

Condition

	On 		Time
	Time		0
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		Flags	OneAtATime
		At	Root
		CHILDFX	:CHILD_Cloud_Trail.fx
	End
End

#########################################################

Condition

	On 		TRIGGERBITS
	TRIGGERBITS	MALE
	Domany 1

	Event
		Ename	MaleAura
		Type	Local
		At	Root
		Flags	OneAtATime
		WHILE	MALE
		CHILDFX	:CHILD_Cloud_Ground_NOFLY.fx
	End
End

Condition

	On 		TRIGGERBITS
	TRIGGERBITS	FEMALE
	Domany 1

	Event
		Ename	FemaleAura
		Type	Local
		At	Root
		Flags	OneAtATime
		WHILE	FEMALE
		CHILDFX	:CHILD_Cloud_Ground_NOFLY.fx
	End
End

Condition

	On 		TRIGGERBITS
	TRIGGERBITS	HUGE
	Domany 1

	Event
		Ename	HugeAura
		Type	Local
		At	Root
		Flags	OneAtATime
		WHILE	HUGE
		CHILDFX	:CHILD_Cloud_Ground_NOFLY.fx
	End
End

#########################################################

End