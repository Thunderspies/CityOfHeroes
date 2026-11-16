#############################################################
## VolcanicGasAttack.fx
#############################################################

FxInfo

Lifespan 50

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	Origin
		Sound rumble2_loop 80 80 0.5
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	ExplosiveForce
		Type	Start
		At	T_Root
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		10
			PhysForcePower		120
			PhysForcePowerJitter	40
		End
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	RockBase
		Type	start
		At	T_Root
		Part	powers\EarthControl\SteamAttackAddative.part
		Part	powers\EarthControl\SteamAttack.part
		Sound Arcoflame 80 80 0.88
	End

	Event
		EName 	RockBase
		Type	start
		At	T_Root
		Lifespan 45
		Part	powers\EarthControl\ViolentSteamCore.part
		Part	powers\EarthControl\ViolentSteam.part
	End

	Event
		EName 	RockBase
		Type	start
		At	T_Root
		Bhvr	behaviors\VolcanicGasRock1.bhvr
		Part	powers\EarthControl\StalagtiteCrack8.part
		Part	powers\EarthControl\GasRadiusRocks.part
		Geom	VolcanicRock01
	End

	Event
		EName 	Dust
		Type	start
		At	T_Root
		Part	powers\EarthControl\VolcanicDust.part
		Part	powers\EarthControl\VolcanicDust2.part
		Part	powers\EarthControl\LavaBubblePopping2.part
		Part	powers\EarthControl\LavaSprayx.part
		Part	powers\EarthControl\LavaBubbleFlat2.part
	End

	Event
		EName 	crack
		Type	start
		At	T_Root
		Bhvr	behaviors\VolcanicGassesCrackScale.bhvr
		geom	Crack01
	End
End

#############################################################

Condition
	On 	Time
	Time 	3

	Event
		EName 	Rock2
		Type	start
		At	T_Root
		Bhvr	behaviors\VolcanicGasRock5.bhvr
		Part	powers\EarthControl\StalagtiteCrack8.part
		Geom	VolcanicRock02
	End
End

Condition
	On 	Time
	Time 	4

	Event
		EName 	Rock3
		Type	start
		At	T_Root
		Bhvr	behaviors\VolcanicGasRock3.bhvr
		Part	powers\EarthControl\StalagtiteCrack8.part
		Geom	VolcanicRock03
	End
End

Condition
	On 	Time
	Time 	6

	Event
		EName 	Rock4
		Type	start
		At	T_Root
		Bhvr	behaviors\VolcanicGasRock4.bhvr
		Part	powers\EarthControl\StalagtiteCrack8.part
		Geom	VolcanicRock04
	End
End

#############################################################

End