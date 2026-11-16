#########################################################
## Super Strength - Foot Stomp
########################################################

FxInfo

lifespan 100

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Streak5
		Type	local
		At	FootR
		Sound ReichsmanStreaksMed_01 100.0 100.0 .75
		Sound ReichsmanStreaksMed_02 100.0 100.0 .75
		Sound ReichsmanStreaksMed_03 100.0 100.0 .75
	End
End

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Streak1
		Type	local
		At	UlegR
		Part1	:StrengthFootGlows.part
		PMagnet	LlegR
		Lifespan 30
	End

	Event
		EName	Streak2
		Type	local
		At	UlegL
		Part1	:StrengthFootGlows.part
		PMagnet	LlegL
		Lifespan 30

	End

	Event
		EName	Streak3
		Type	local
		At	LlegR
		Part1	:StrengthFootGlows.part	#:StrengthLowerFootGlows.part
		PMagnet	FootR
		Lifespan 42
	End

	Event
		EName	Streak4
		Type	local
		At	LlegL
		Part1	:StrengthFootGlows.part #:StrengthLowerFootGlows.part
		PMagnet	FootL
		Lifespan 42
	End


	Event
		EName	Streak5
		Type	local
		At	FootR
		Part1	:StrengthStreak.part
		Lifespan 30
	End

	Event
		EName	Streak6
		Type	local
		At	FootL

		Part1	:StrengthStreak.part
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	26

	Event
		EName	flash
		Type	StartPositOnly
		At	FootR
		BhvrOverride
			PositionOffset 0.0 -.6 0.0
		End
		Part3	:FootStompStreaks.part
		Part4	:LightBurst.part
		Part5	:LightBurstFlat.part
		Sound ReichsmanFootStomp_01 100.0 100.0 .85
	End
End

Condition
	On 	Time
	Time 	26

	Event
		EName	flash
		Type	start
		At	FootR

		Bhvr	Behaviors/CameraShake01.bhvr
	End
End

Condition
	On 	Time
	Time 	23

	Event
		EName	crack
		Type	start
		At	FootR
		Bhvr	POWERS/SuperStrength/Splat_FootStomp.bhvr
		Splat	ImpactCracks.tga
		Lifespan	40
	End

	Event
		EName	flash2
		Type	StartPositOnly
		At	FootR
		BhvrOverride
			PositionOffset 0.0 -.6 0.0
		End
		Part2	:FootStompRing.part
		Lifespan	25
	End

End

## PHYSICS FORCES ######################################################

Condition
	On 	Time
	Time 	29

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Mystic

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		60
			PhysForcePower		100
			PhysForcePowerJitter	50
		End
		Lifespan	1
	End

	Event
		Ename	BounceForce
		Type	Start
		At	Mystic

		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		55
			PhysForcePower		500
			PhysForcePowerJitter	100
		End
		Lifespan	1
	End
End



########################################################

End