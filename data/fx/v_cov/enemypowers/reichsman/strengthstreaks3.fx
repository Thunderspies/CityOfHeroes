#########################################################
## Super Strength - Hand Clap
########################################################

FxInfo

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Streak1
		Type	local
		At	UARMR
		Part1	:StrengthBodyGlows.part
		PMagnet	LarmR
	End

	Event
		EName	Streak1
		Type	local
		At	UARML
		Part1	:StrengthBodyGlows.part
		PMagnet	LarmL
	End

	Event
		EName	Streak1
		Type	local
		At	LarmR
		Part1	:StrengthBodyGlows.part
		PMagnet	WepR
	End

	Event
		EName	Streak1
		Type	local
		At	LarmL
		Part1	:StrengthBodyGlows.part
		PMagnet	WepL
	End


	Event
		EName	Streak1
		Type	local
		At	WepR
		Part1	:StrengthStreak.part
		Part3	:StrengthGlow.part
		Sound ReichsmanStreaks_01 100.0 100.0 .55
		Sound ReichsmanStreaks_02 100.0 100.0 .55
		Sound ReichsmanStreaks_03 100.0 100.0 .55
	End

	Event
		EName	Streak1
		Type	local
		At	WepL
		Part1	:StrengthStreak.part
		Part3	:StrengthGlow.part
	End
End

########################################################

Condition
	On 	Time
	Time 	23

	Event
		EName 	Mallet1
		Type	start
		At	mystic
		bhvr	behaviors/ScaleBubble3.bhvr
		BhvrOverride
			RGB0 0 0 255
		End
		Geom	GEO_WhiteSphere
		Lifespan 7
	End

	Event
		EName 	Mallet2
		Type	start
		At	mystic
		Part1	:SuperStrengthHitEmber.part
		Part2	:SuperStrengthHitStar.part
		Part3	:SuperStrengthHitRing.part
		Part4	:SuperStrengthHitRing2.part
		Part5	:SuperStrengthHitGlows2.part
		Sound PunchHit2 100.0 100.0 .8
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	mystic
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		300
			PhysForcePowerJitter	100
		End
		Lifespan	1
	End
End

########################################################

Condition
	On 	Time
	Time	26

	Event
		Ename	Offset
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset		0 2 0
		End
	End

	Event
		EName 	CameraShake
		Type	Start
		At	Offset
		Bhvr	Behaviors/CameraShake01.bhvr
		BhvrOverride
			Shake		2.0	#How much to shake the camera when event with this bhvr is created
			ShakeFallOff	0.25	#how long shake lasts
			ShakeRadius	48	#(falls off slowly, so at 18.0 feet from center, only a mild shake)
		End
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Offset
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		200
			PhysForcePowerJitter	100
		End
		Lifespan	1
	End
End

########################################################

Condition
	On 	Time
	Time 	37

	Event
		EName	Mallet1
		Type	Destroy
	End

	Event
		EName	Mallet2
		Type	Destroy
	End
End

Condition
	On 	Time
	Time 	40

	Event
		EName	All
		Type	Destroy
	End
End

########################################################

End