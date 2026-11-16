#########################################################
## Super Strength - Foot Stomp
########################################################

FxInfo

Lifespan 100

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Streak5
		Type	local
		At	FootR
		Sound footstomp2 100.0 100.0 0.85
	End
End

########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	UlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\SuperStrength\StrengthFootGlows.part
		PMagnet	LlegR
		Lifespan 30
	End

	Event
		Type	local
		At	UlegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\SuperStrength\StrengthFootGlows.part
		PMagnet	LlegL
		Lifespan 30
	End

	Event
		Type	local
		At	LlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\SuperStrength\StrengthLowerFootGlows.part
		PMagnet	FootR
		Lifespan 42
	End

	Event
		Type	local
		At	LlegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\SuperStrength\StrengthLowerFootGlows.part
		PMagnet	FootL
		Lifespan 42
	End

	Event
		Type	local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\SuperStrength\StrengthStreak.part
		Lifespan 30
	End

	Event
		Type	local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\SuperStrength\StrengthStreak.part
		Lifespan 30
	End
End

#############################################################

Condition
	On 	Time
	Time 	22

	Event
		EName	crack
		Type	start
		At	Mystic
		Bhvr	CustomizeablePowers\SuperStrength\Splat_FootStomp.bhvr
		Splat	ImpactCracks.tga
		Lifespan 40
	End

	Event
		Type	start
		At	Mystic
		Part2	CustomizeablePowers\SuperStrength\FootStompRing.part
		Lifespan 25
	End
End

Condition
	On 	Time
	Time 	20

	Event
		EName	flash
		Type	start
		At	mystic
		Part3	CustomizeablePowers\SuperStrength\FootStompStreaks.part
		Part4	CustomizeablePowers\SuperStrength\LightBurst.part
		Part5	CustomizeablePowers\SuperStrength\LightBurstFlat.part
	End

	Event
		EName	flash
		Type	start
		At	mystic
		Bhvr	CustomizeablePowers\SuperStrength\CameraShake01.bhvr
	End
End

#############################################################

Condition
	On 	Time
	Time 	22

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
		Lifespan 1
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
		Lifespan 1
	End
End

#############################################################

End