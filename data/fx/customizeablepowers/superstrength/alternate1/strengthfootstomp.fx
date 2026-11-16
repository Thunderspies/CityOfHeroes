#########################################################
## Super Strength - Foot Stomp
########################################################

FxInfo

Lifespan 100

########################################################

Condition
	On 	Time
	Time 	7

	Event
		EName	Streak5
		Type	local
		At	Mystic
		Sound footstomp2 100.0 100.0 0.85
	End
End

########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	UArmR
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	:StrengthFootGlows.part
		PMagnet	LArmR
		Lifespan 25
	End

	Event
		Type	local
		At	UArmL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	:StrengthFootGlows.part
		PMagnet	LArmL
		Lifespan 25
	End

	Event
		Type	local
		At	LArmR
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	:StrengthLowerFootGlows.part
		PMagnet	WepR
		Lifespan 25
	End

	Event
		Type	local
		At	LArmL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	:StrengthLowerFootGlows.part
		PMagnet	WepL
		Lifespan 25
	End

	Event
		EName	Streak5
		Type	local
		At	WepR
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\SuperStrength\StrengthStreak.part
		Lifespan 25
	End

	Event
		EName	Streak6
		Type	local
		At	WepL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\SuperStrength\StrengthStreak.part
		Lifespan 25
	End
End

#############################################################

Condition
	On 	Time
	Time 	28

	Event
		EName	flash
		Type	Start
		At	Mystic
		Bhvr	CustomizeablePowers\SuperStrength\CameraShake01.bhvr
		BhvrOverride
			PositionOffset	0 1 0
		End
		Part3	CustomizeablePowers\SuperStrength\FootStompStreaks.part
		Part4	CustomizeablePowers\SuperStrength\LightBurst.part
		Part5	CustomizeablePowers\SuperStrength\LightBurstFlat.part
	End
End

Condition
	On 	Time
	Time 	28

	Event
		EName	crack
		Type	Start
		At	Mystic
		Bhvr	CustomizeablePowers\SuperStrength\Splat_FootStomp.bhvr
		Splat	ImpactCracks.tga
		Lifespan 40
	End

	Event
		EName	flash2
		Type	StartPositOnly
		At	Mystic
		BhvrOverride
			PyrRotate 0 0 0
		End
		Part2	CustomizeablePowers\SuperStrength\FootStompRing.part
		Lifespan 25
	End
End

## PHYSICS FORCES ######################################################

Condition
	On 	Time
	Time 	28

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

########################################################

End