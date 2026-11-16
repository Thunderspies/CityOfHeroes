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
		Part	Scripted\Praetorian\Murder_Of_Crows_Teleport\Feathers_Burst.part
		Part	Scripted\Praetorian\Murder_Of_Crows_Teleport\Smoke_Burst.part
		Lifespan 12
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

		ChildFX	Scripted\Praetorian\Murder_Of_Crows_Teleport\CHILD_MurderOfCrows_TeleportOut.fx
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
		ChildFX	Scripted\Praetorian\Murder_Of_Crows_Teleport\CHILD_MurderOfCrows_TeleportOut.fx
		Lifespan 80
	End
End

Condition
	On	Time
	Time	3

	Event
		Type	PositOnly
		At	Origin
		Part	Scripted\Praetorian\Murder_Of_Crows_Teleport\Crows_Fleeing.part
		Part	Scripted\Praetorian\Murder_Of_Crows_Teleport\Crows_Fleeing_Strobing.part
		Lifespan 8
	End
End

#############################################################

End