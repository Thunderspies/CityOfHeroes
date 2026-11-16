#############################################################
## PetrifyingGaze.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		EName 	InitialHeadMist
		Type	start
		At	head

		part1	POWERS/DarknessControl/bFearMist.part
		part2	POWERS/DarknessControl/bFearMist2.part
		Part3	POWERS/DarknessControl/bFearTenticles.part
		Part4	POWERS/DarknessControl/bFearTenticles2.part

		Magnet	Target
		Lookat	Target
		Sound petrify1 80 80 .8

	End

	Event
		EName 	Center
		Type	create
		Inherit	position
		Update	position
		At	Head

		Lookat	target
	End

	Event
		EName 	Right
		Type	Local
		At	Center

		Magnet	Center

		BhvrOverride
			PositionOffset	-13 0 0
		End
	End

	Event
		EName 	Left
		Type	Local
		At	Center

		Magnet	Center

		BhvrOverride
			PositionOffset	13 0 0
		End
	End

End

Condition
	On 	Time
	Time 	30


	Event
		EName 	Rtarg
		Type	Start
		At	Right

		Magnet	Target

		BhvrOverride
			Trackrate	5
		End
	End

	Event
		EName 	Ltarg
		Type	Start
		At	Left

		Magnet	Target

		BhvrOverride
			Trackrate	5
		End
	End

	Event
		EName 	Prime
		Type	start
		At	head

		Bhvr	behaviors\slowprjct1TOROOT.bhvr
		BhvrOverride
			InitialVelocity 0 0 0
		End
		Part5	POWERS\DarknessControl\PetrifingPuddleTrail2.Part
		part1	POWERS\DarknessControl\aFearMist.part
		part2	POWERS\DarknessControl\aFearMist2.part
		Part3	POWERS\DarknessControl\aFearTenticles.part
		Part4	POWERS\DarknessControl\aFearTenticles2.part


		Magnet	Rtarg
		Lookat	Rtarg

	End

	Event

		EName 	Prime1
		Type	start
		At	head

		Bhvr	behaviors\slowprjct1TOROOT2.bhvr
		BhvrOverride
			InitialVelocity 0 0 0
		End
		Part5	POWERS\DarknessControl\PetrifingPuddleTrail2.Part
		part1	POWERS\DarknessControl\aFearMist.part
		part2	POWERS\DarknessControl\aFearMist2.part

		Part3	POWERS\DarknessControl\aFearTenticles.part
		Part4	POWERS\DarknessControl\aFearTenticles2.part


		Magnet	Ltarg
		Lookat	Ltarg
	End

End

Condition
	On 	time
	Time	40

	Event
		EName 	InitialHeadMist
		Type	Destroy

	End

End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy

	End

End

Condition
	On 	Prime1Hit

	Event
		EName 	Prime1
		Type	Destroy

	End

End

End