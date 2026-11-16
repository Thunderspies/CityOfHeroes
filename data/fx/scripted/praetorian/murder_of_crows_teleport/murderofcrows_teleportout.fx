#############################################################
## MurderOfCrows_TeleportIn.bhvr
#############################################################

FxInfo
Lifespan 100

#############################################################

Input
	InpName	Hips
End

#############################################################
Condition
	On	Time
	Time	0

	Event
		Type	PositOnly
		At	Root
		Part	:Feathers_Burst.part
		Part	:Smoke_Burst.part
		Lifespan 12
	End

	Event
		Type	PositOnly
		At	Root
		Bhvr	behaviors/FadeOut.bhvr
		BhvrOverride
			FadeOutLength	10
		End
		Flags	AdoptParentEntity
		Lifespan 1
	End
End

Condition
	On 	Time
	Time 	0
	Repeat	5

	Event
		EName 	Prime
		Type	Start
		At	Origin

		ChildFX	:CHILD_MurderOfCrows_TeleportOut.fx
		Lifespan 80
	End
End
Condition
	On	Time
	Time	5

	Event
		EName 	CrowAnchor
		Type	Start
		At	Origin
		Lifespan 12
	End
End

Condition
	On 	Cycle
	Time 	1

	Event
		EName 	Prime
		Type	Start
		At	CrowAnchor
		ChildFX	:CHILD_MurderOfCrows_TeleportOut.fx
		Lifespan 80
	End
End

Condition
	On	Time
	Time	3

	Event
		Type	PositOnly
		At	Origin
		Part	:Crows_Fleeing.part
		Part	:Crows_Fleeing_Strobing.part
		Lifespan 8
	End
End

#############################################################

End