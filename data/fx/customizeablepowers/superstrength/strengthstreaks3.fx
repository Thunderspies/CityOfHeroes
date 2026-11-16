#########################################################
## Super Strength - Hand Clap
########################################################

FxInfo

Lifespan 40

########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	UARMR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\SuperStrength\StrengthBodyGlows.part
		PMagnet	LarmR
	End

	Event
		Type	local
		At	UARML
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\SuperStrength\StrengthBodyGlows.part
		PMagnet	LarmL
	End

	Event
		Type	local
		At	LarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\SuperStrength\StrengthBodyGlows.part
		PMagnet	WepR
	End

	Event
		EName	Streak1
		Type	local
		At	LarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\SuperStrength\StrengthBodyGlows.part
		PMagnet	WepL
	End

	Event
		EName	Streak1
		Type	local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\SuperStrength\StrengthStreak.part
		Part3	CustomizeablePowers\SuperStrength\StrengthGlow.part
		Sound handclapSSCustom 100.0 100.0 0.85
	End

	Event
		EName	Streak1
		Type	local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\SuperStrength\StrengthStreak.part
		Part3	CustomizeablePowers\SuperStrength\StrengthGlow.part
	End
End

########################################################

Condition
	On 	Time
	Time 	23

	Event
		Type	start
		At	mystic
		bhvr	CustomizeablePowers\SuperStrength\ScaleBubble.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_SuperStrengthBubble
		Lifespan 14
	End

	Event
		Type	start
		At	mystic
		Part1	CustomizeablePowers\SuperStrength\SuperStrengthHitEmber.part
		Part2	CustomizeablePowers\SuperStrength\SuperStrengthHitStar.part
		Part3	CustomizeablePowers\SuperStrength\SuperStrengthHitRing.part
		Part4	CustomizeablePowers\SuperStrength\SuperStrengthHitRing2.part
		Part5	CustomizeablePowers\SuperStrength\SuperStrengthHitGlows2.part
		Sound PunchHit2 100.0 100.0 0.8
		Lifespan 14
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
		Lifespan 1
	End
End

########################################################

Condition
	On 	Time
	Time	26

	Event
		EName 	CameraShake
		Type	Start
		At	Mystic
		Bhvr	CustomizeablePowers\SuperStrength\CameraShake01.bhvr
		BhvrOverride
			Shake		2.0	#How much to shake the camera when event with this bhvr is created
			ShakeFallOff	0.25	#how long shake lasts
			ShakeRadius	48	#(falls off slowly, so at 18.0 feet from center, only a mild shake)
		End
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Mystic
		BhvrOverride
			PositionOffset		0 2 0
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		200
			PhysForcePowerJitter	100
		End
		Lifespan 1
	End
End

#############################################################

End