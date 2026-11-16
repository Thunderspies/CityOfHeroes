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
		Until	AIR
		At	Root
		Part	:Cloud_Ground.part
		Part	:Cloud_Ground_Highlights.part
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
		Until	AIR
		At	Root
		Part	:Cloud_Ground.part
		Part	:Cloud_Ground_Highlights.part
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
		Until	AIR
		At	Root
		Part	:Cloud_Ground.part
		Part	:Cloud_Ground_Highlights.part
	End

End

#########################################################

End