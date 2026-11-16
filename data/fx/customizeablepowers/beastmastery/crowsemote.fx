#############################################################
## MurderOfCrows_TeleportIn.bhvr
#############################################################

FxInfo
Lifespan 200

#############################################################

Input
	InpName	Hips
End

#############################################################

Condition
	Event
		Type	Local
		At	Root
		Sound 	MurderOfCrows_01 100 100 .8
	End
End


Condition
	On 	Time
	Time 	0
	Repeat	3

	Event
		EName 	Prime
		Type	Start
		At	Origin
		ChildFX	:CHILD_MurderOfCrows_TeleportIn.fx
		Lifespan 40
	End
End

Condition
	On	Time
	Time	5
	Event
		EName 	Prime
		Type	Start
		At	Origin
		Part	:Crows_Gathering.part

		PMagnet	Head
		Lifespan 10
	End

	Event
		EName 	Prime
		Type	Start
		At	Origin
		Part	:Crows_Gathering2.part

		PMagnet	Head
		Lifespan 12
	End
End

Condition
	On 	Time
	Time 	12

	Event
		EName 	Prime
		Type	Start
		At	Root
		Part	:Smoke_Gathering.part
		Lifespan 28
		POther	Head
	End
End

Condition
	On 	Time
	Time 	20

	Event
		EName 	Prime
		Type	Start
		At	Root

		Part	:Dust_Swirling.part
		Lifespan 10
		POther	Head
	End


	Event
		EName 	Prime
		Type	Start
		At	Root
		Part	:Crows_Swirling.part
		Part	:Crows_Swirling2.part

		Lifespan 15
		PMagnet	Head
	End
End

#############################################################

Condition
	On	Time
	Time	45

	Event
		Type	PositOnly
		At	Root
		Part	:Feathers_Burst.part
		Part	:Smoke_Burst.part
		Lifespan 12
	End
End

Condition
	On 	Time
	Time 	45
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
	Time	50

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
	Time	48

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