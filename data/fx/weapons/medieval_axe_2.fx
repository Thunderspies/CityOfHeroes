#########################################################
## Undead Slaying Axe
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName 	InitialAxe
		Type	Local
		At	WepR
		Bhvr	Behaviors\Weapon_Fade_UP.bhvr
		ChildFX	:Child_MedievalAxe2.fx
		Sound axeout 70 70 0.6
	End
End

Condition
	On	TriggerBits
	TriggerBits	NONHUMAN

	Event
		EName 	InitialAxe
		Type	Destroy
	End
End

#############################################################

Condition
	On	TriggerBits
	TriggerBits	MALE
	DoMany	1

	Event
		EName 	Male_PostFormShift
		Type	Local
		At	WepR
		Flags	OneAtATime
		While	MALE
		ChildFX	:Child_MedievalAxe2.fx
	End
End

Condition
	On	TriggerBits
	TriggerBits	FEMALE
	DoMany	1

	Event
		EName 	Female_PostFormShift
		Type	Local
		At	WepR
		Flags	OneAtATime
		While	FEMALE
		ChildFX	:Child_MedievalAxe2.fx
	End
End

Condition
	On	TriggerBits
	TriggerBits	HUGE
	DoMany	1

	Event
		EName 	Huge_PostFormShift
		Type	Local
		At	WepR
		Flags	OneAtATime
		While	HUGE
		ChildFX	:Child_MedievalAxe2.fx
	End
End

#########################################################

End