#########################################################
## Stone Control - Stalagmite Hit
#########################################################

FxInfo

LifeSpan	200

## SOUND FX #######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Streak1
		Type	local
		At	origin

		Sound punch13 100.0 100.0 .65
	End

End

Condition
	On 	Time
	Time 	23

	Event
		EName	prime
		Type	start
		At	mystic

		Sound quake2 120 120 .9
	End
End

#########################################################

Condition
	On 	Time
	Time 	30

	Event
		EName	Quake
		Type	local
		At	root

		bhvr	CustomizeablePowers\EarthControl\SlightQuake.bhvr
	End
End

Condition
	On 	Time
	Time 	23

	Event
		EName	CameraShake
		Type	start
		At	mystic

		Bhvr	CustomizeablePowers\EarthControl\CameraShakeStomp.bhvr
	End
End

Condition
	On 	Time
	Time 	20

	Event
		EName	cracks
		Type	start
		At	mystic

		Part2	CustomizeablePowers\EarthControl\superStrengthDust1.part
		part3	CustomizeablePowers\EarthControl\superStrengthDust2.part
	End
End

Condition
	On	Time
	Time	23

	Event
		EName	crack
		Type	start
		At	mystic

		Bhvr	CustomizeablePowers\EarthControl\StalagmiteCracks.bhvr
		Splat	ImpactCracks.tga

		Lifespan	100
	End
End

#########################################################

Condition
	On 	Time
	Time 	23

	Event
		EName	prime
		Type	start
		At	mystic

		Part	CustomizeablePowers\EarthControl\StalagmiteRocks01x.part
		Part	CustomizeablePowers\EarthControl\StalagmiteRocks02x.part
	End
End

## PHYSICS FORCES #######################################################

Condition
	On 	Time
	Time 	23

	Event
		Ename	ExplosiveForce
		Type	Start
		At	mystic

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		60
			PhysForcePower		200
			PhysForcePowerJitter	100
		End

		Lifespan	1
	End
End

## CLEAN UP #######################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End
End

#########################################################

End