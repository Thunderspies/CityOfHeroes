#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################

Condition

	On 		TRIGGERBITS
	TRIGGERBITS	MALE
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		Flags	OneAtATime
		Until	FLY
		At	Root
		CHILDFX	:CHILD_StormCloud_Ground_NOAIR.fx
	End

End

Condition

	On 		TRIGGERBITS
	TRIGGERBITS	FEMALE
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		Flags	OneAtATime
		Until	FLY
		At	Root
		CHILDFX	:CHILD_StormCloud_Ground_NOAIR.fx
	End
End

Condition

	On 		TRIGGERBITS
	TRIGGERBITS	HUGE
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		Flags	OneAtATime
		Until	FLY
		At	Root
		CHILDFX	:CHILD_StormCloud_Ground_NOAIR.fx
	End

End
#########################################################

End