#############################################################
## BonfireCast.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepR
		Part1	:FireHands.part
		Sound Firehand_loop 60.0 60.0 .45
		LifeSpan	100
	End

	Event
		EName	InitialSwordFire1
		Type	Local
		At	WepL
		Part1	:FireHands.part
		Sound Ignite2 60.0 60.0 .8
		LifeSpan	100
	End
End

Condition
	On 	Time
	Time 	30

	Event

		EName 	LocknarDummyHookup
		Type	Start
		At	Root
		Geom	FX_InfectOthers
	End

	Event
		EName 	LocknarFadeIn
		Type	Start
		At	LocknarDummyHookup
		 AltPiv 1

		Part1	:BonfireStart.part
		Part3	:BonfireDotsHands.part
		Lifespan	25
	End
End

Condition
	On	time
	time	63

	Event
		Ename	LocknarDummyHookup
		Type	Destroy
	End

	Event
		Ename	LocknarFadeIn
		Type	Destroy
	End
End

#############################################################

Condition
	On	time
	time	61

	Event
		EName 	Prime
		Type	Start
		At	LocknarDummyHookup
		AltPiv	1
		Bhvr	behaviors\MorterLobbprojectile.bhvr
		Part1	:BonfireStreak.part
		Part2	POWERS\RadiationControl\RadiationHandElectrons.part
		Part3	:BonfireStreakGlows.part
		Sound firestreaks 80 80 .88
		Magnet	Target
		LookAt	Target
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End
End

#############################################################

End