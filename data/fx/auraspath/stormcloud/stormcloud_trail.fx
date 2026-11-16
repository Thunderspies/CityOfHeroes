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
		CHILDFX	:CHILD_StormCloud_Trail.fx
	End
End

#########################################################

Condition

	On 		TRIGGERBITS
	TRIGGERBITS	MALE

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		WHILE	MALE
		CHILDFX	:CHILD_StormCloud_Ground_NOFLY.fx
	End
End

Condition

	On 		TRIGGERBITS
	TRIGGERBITS	FEMALE

	Event
		ENAME	Cloud
		Type	Local
		WHILE	FEMALE
		At	Root
		CHILDFX	:CHILD_StormCloud_Ground_NOFLY.fx
	End
End

Condition

	On 		TRIGGERBITS
	TRIGGERBITS	HUGE

	Event
		ENAME	Cloud
		Type	Local
		WHILE	HUGE
		At	Root
		CHILDFX	:CHILD_StormCloud_Ground_NOFLY.fx
	End
End

#########################################################

End