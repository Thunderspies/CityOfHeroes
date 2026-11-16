#############################################################
## Header
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	UarmL
		Part1	:FireArms.part
		Sound Firehand_loop 100.0 100.0 .55
		LifeSpan	20
	End

	Event
		Type	Local
		At	UarmR
		Part1	:FireArms.part
		Sound Ignite2 100.0 100.0 .9
		LifeSpan	20
	End
End

Condition
	On 	Time
	Time 	15

	Event
		EName	InitialSwordFire
		Type	Local
		At	LarmR
		Part1	:FireArms.part
		LifeSpan	20
	End

	Event
		EName	InitialSwordFire1
		Type	Local
		At	LarmL
		Part1	:FireArms.part
		LifeSpan	20
	End
End

Condition
	On 	Time
	Time 	20

	Event
		Type	Local
		At	HandR
		Part1	:FireHandzA.part
		LifeSpan	20
	End

	Event
		Type	Local
		At	HandL
		Part1	:FireHandzA.part
		LifeSpan	20
	End
End

#############################################################

Condition
	On 	Time
	Time 	25

	Event
		EName 	Prime
		Type	Start
		At	Mystic
		Sound fireball2 80 80 .9
		LifeSpan	60
	End
End

#############################################################

End