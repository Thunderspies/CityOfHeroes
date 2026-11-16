#############################################################
## InfectOthers.fx
#############################################################

FxInfo

Lifespan 200

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Hand1
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\RadiationBlast\RadiationInfectOthersHands.part
		Part2	CustomizeablePowers\RadiationBlast\RadiationHandElectrons.part
		Part3	CustomizeablePowers\RadiationBlast\InfectBubbleDotsHands.part
		Part4	CustomizeablePowers\RadiationBlast\InfectBubblesHands.part
		PMagnet	WepR
		Sound radiation3_loop 80 80 .4
		Lifespan 100
	End

	Event
		EName	Hand2
		Type	Local
		At	WepL
		Part1	CustomizeablePowers\RadiationBlast\RadiationInfectOthersHands.part
		Part2	CustomizeablePowers\RadiationBlast\RadiationHandElectrons.part
		Part3	CustomizeablePowers\RadiationBlast\InfectBubbleDotsHands.part
		Part4	CustomizeablePowers\RadiationBlast\InfectBubblesHands.part
		PMagnet	WepL
		Lifespan 100
	End
End

#############################################################

Condition
	On 	Time
	Time 	30

	Event
		EName	LocknarDummyHookup
		Type	Start
		At	Head
		BhvrOverride
			PositionOffset	0.0 1.75 0.0
		End
		Sound radblast 80 80 .85
		Lifespan 35
	End

	Event
		Type	Start
		At	LocknarDummyHookup
		Bhvr	CustomizeablePowers\RadiationBlast\InfectOthersFadeIn.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part1	CustomizeablePowers\RadiationBlast\InfectOthersLocknar.part
		Part2	CustomizeablePowers\RadiationBlast\RadiationHandElectrons.part
		Part3	CustomizeablePowers\RadiationBlast\InfectBubbleDotsHands.part
		Part4	CustomizeablePowers\RadiationBlast\InfectBubblesHands.part
		Part5	CustomizeablePowers\RadiationBlast\RadiationInfectOthersHands.part
		Geom	Tintable_FX_Locknar
		Lifespan 33
	End
End

#############################################################

Condition
	On 	Time
	Time 	63

	Event
		EName 	Prime
		Type	Start
		At	LocknarDummyHookup
		Bhvr	CustomizeablePowers\RadiationBlast\MorterLobbprojectile.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part1	CustomizeablePowers\RadiationBlast\InfectOthersStreak.part
		Part2	CustomizeablePowers\RadiationBlast\RadiationHandElectrons.part
		Part3	CustomizeablePowers\RadiationBlast\MorterStreakGlows.part
		Part4	CustomizeablePowers\RadiationBlast\MorterStreakRings.part
		Geom	Tintable_FX_Locknar
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

	Event
		Ename	Mushroom
		Type	Local
		At	T_Root
		ChildFx CustomizeablePowers\RadiationBlast\InfectOthersHit.fx
	End
End

#############################################################

End