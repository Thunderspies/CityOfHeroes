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
	On 	Time
	Time 	0
	Repeat	2

	Event
		EName 	Prime
		Type	Start
		At	Origin
		ChildFX	:CHILD_MurderOfCrows_TeleportIn.fx
		Lifespan 80
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
		Lifespan 15
	End

	Event
		EName 	Prime
		Type	Start
		At	Origin
		Part	:Crows_Gathering2.part

		PMagnet	Head
		Lifespan 20
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
		Lifespan 20
		POther	Head
	End


	Event
		EName 	Prime
		Type	Start
		At	Root
		Part	:Crows_Swirling.part
		Part	:Crows_Swirling2.part

		Lifespan 30
		PMagnet	Head
	End
End

#############################################################

End