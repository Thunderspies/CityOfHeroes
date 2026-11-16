#############################################################
## SoulDrain_Attack.fx
#############################################################

FxInfo

Input
	InpName	Hips
End

LifeSpan	70

###########################################################

Condition
	On	Time
	Time	3
	Event
		EName 	Rbinder7
		Type	local
		At	UarmR
		Bhvr	V_COV\EnemyPowers\GhostWidow\soulBinderArm2.bhvr
		Lifespan	20
	End

	Event
		EName 	ROffset7
		Type	local
		At	Rbinder7
		Bhvr	V_COV\EnemyPowers\GhostWidow\binderOffset.bhvr
		Part	:soulGlow0.part
		Part	:soulFire0.part
		Part	:soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1
		Type	local
		At	ROffset7
		  AltPiv	2
		Part	:soulFire0.part
	End

	Event
		EName 	Arm2
		Type	local
		At	ROffset7
		  AltPiv	4
		Part	:soulFire0.part
	End

End

Condition
	On	Time
	Time	11
	Event
		EName 	Rbinder9
		Type	local
		At	LarmR
		Bhvr	V_COV\EnemyPowers\GhostWidow\soulBinderArm2.bhvr

		Lifespan	20
	End

	Event
		EName 	ROffset9
		Type	local
		At	Rbinder9
		Bhvr	:binderOffset.bhvr
		Part	:soulGlow0.part
		Part	:soulFire0.part
		Part	:soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1
		Type	local
		At	ROffset9
		  AltPiv	2
		Part	:soulFire0.part
	End

	Event
		EName 	Arm2
		Type	local
		At	ROffset9
		  AltPiv	4
		Part	:soulFire0.part
	End

End


Condition
	On	Time
	Time	13

	Event
		EName 	HandR
		Type	local
		At	WepR
		Part	:soulGlow1.part
		Part	:soulFire2.part
		Part	:soulSmoke1.part

		Lifespan	9
	End


End

###########################
#	Left Arm	  #
###########################

Condition
	On	Time
	Time	3
	Event
		EName 	Lbinder7
		Type	local
		At	UarmL
		Bhvr	V_COV\EnemyPowers\GhostWidow\soulBinderArm2.bhvr
			BhvrOverride
				InitialVelocity		-0.02 0.0 0.0
				InitialVelocityJitter	0.005 0.00 0.00
			End
		Lifespan	20
	End

	Event
		EName 	LOffset7
		Type	local
		At	Lbinder7
		Bhvr	V_COV\EnemyPowers\GhostWidow\binderOffset.bhvr
			BhvrOverride
				PositionOffset		-1 0 2
				Spin			0.0 0.0 0.1
				SpinJitter		0.0 0.0 0.05
			End
		Part	:soulGlow0.part
		Part	:soulFire0.part
		Part	:soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	LArm1
		Type	local
		At	LOffset7
		  AltPiv	2
		Part	:soulFire0.part
	End

	Event
		EName 	LArm2
		Type	local
		At	LOffset7
		  AltPiv	4
		Part	:soulFire0.part
	End

End

Condition
	On	Time
	Time	11
	Event
		EName 	Lbinder9
		Type	local
		At	LarmL
		Bhvr	V_COV\EnemyPowers\GhostWidow\soulBinderArm2.bhvr
			BhvrOverride
				InitialVelocity		-0.02 0.0 0.0
				InitialVelocityJitter	0.005 0.00 0.00
			End
		Lifespan	20
	End

	Event
		EName 	LOffset9
		Type	local
		At	Lbinder9
		Bhvr	:binderOffset.bhvr
			BhvrOverride
				PositionOffset		-1 0 2
				Spin			0.0 0.0 0.1
				SpinJitter		0.0 0.0 0.05
			End
		Part	:soulGlow0.part
		Part	:soulFire0.part
		Part	:soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	LArm1
		Type	local
		At	LOffset9
		  AltPiv	2
		Part	:soulFire0.part
	End

	Event
		EName 	LArm2
		Type	local
		At	LOffset9
		  AltPiv	4
		Part	:soulFire0.part
	End

End


Condition
	On	Time
	Time	13

	Event
		EName 	HandL
		Type	local
		At	WepL
		Part	:soulGlow1.part
		Part	:soulFire2.part
		Part	:soulSmoke1.part

		Lifespan	9
	End


End




##########################





Condition
	On	Time
	Time	0

	Event
		EName	x
		Type	start
		At	Root
		part1	CustomizeablePowers\DarkMelee\TendrilRing.part
		part2	CustomizeablePowers\DarkMelee\TendrilRingFlat.part
		Sound Darkmoonbeam3 80 80 .8

		LifeSpan	46
	End

End

Condition
	On	Time
	Time	43

	Event
		EName	x
		Type	start
		At	Root
		part1	CustomizeablePowers\DarkMelee\TendrilRingFlatBurst.part
		part2	CustomizeablePowers\DarkMelee\TendrilRingFlatBurst2.part


		LifeSpan	3
	End

End

Condition
	On	Time
	Time	45

	Event
		EName	y
		Type	start
		At	Root

		Splat	Generic_Ring.tga
		Bhvr	Powers\DarknessControl\BlackYellowRingIn.bhvr

		LifeSpan	10
	End

End

Condition
	On	Time
	Time	49

	Event
		EName	y
		Type	start
		At	Root

		Splat	ThinRing.tga
		Bhvr	Powers\DarknessControl\BlackYellowRingIn2.bhvr

		LifeSpan	10
	End

End

Condition
	On	Time
	Time	53

	Event
		EName	y
		Type	start
		At	Root

		Splat	Generic_Ring.tga
		Bhvr	Powers\DarknessControl\BlackYellowRingIn3.bhvr

		LifeSpan	10
	End

End

Condition
	On	Time
	Time	55

	Event
		EName	y
		Type	start
		At	Root

		Splat	ThinRing.tga
		Bhvr	Powers\DarknessControl\BlackYellowRingIn4.bhvr

		LifeSpan	10
	End
End

Condition

	Event
		EName	15
		Type	start
		At	Chest
		part1	CustomizeablePowers\DarkMelee\HighLightRays.part
		part2	CustomizeablePowers\DarkMelee\HighLightStreaks.part
		part3	CustomizeablePowers\DarkMelee\HighLightRaysMulticolor.part
		part4	CustomizeablePowers\DarkMelee\HighLightStreaksMulticolor.part
	End

End

#############################################################

End