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
		Part	:SteamAttackAddative.part
		Part	:SteamAttack.part
		Sound Arcoflame 80 80 0.88
	End

	Event
		EName 	RockBase
		Type	start
		At	T_Root
		Lifespan 45
		Part	:ViolentSteamCore.part
		Part	:ViolentSteam.part
	End

	Event
		EName 	RockBase
		Type	start
		At	T_Root
		Bhvr	behaviors\VolcanicGasRock1.bhvr
		BhvrOverride
			TintGeom	1
		End
		Part	powers\EarthControl\StalagtiteCrack8.part
		Part	:GasRadiusRocks.part
		Geom	Tintable_VolcanicRock01
	End

	Event
		EName 	Dust
		Type	start
		At	T_Root
		Part	:VolcanicDust.part
		Part	:VolcanicDust2.part
		Part	:LavaBubblePopping2.part
		Part	:LavaSprayx.part
		Part	:LavaBubbleFlat2.part
	End

	Event
		EName 	crack
		Type	start
		At	T_Root
		Bhvr	behaviors\VolcanicGassesCrackScale.bhvr
		BhvrOverride
			TintGeom	1
		End
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
		BhvrOverride
			TintGeom	1
		End
		Part	powers\EarthControl\StalagtiteCrack8.part
		Geom	Tintable_VolcanicRock02
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
		BhvrOverride
			TintGeom	1
		End
		Part	powers\EarthControl\StalagtiteCrack8.part
		Geom	Tintable_VolcanicRock03
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
		BhvrOverride
			TintGeom	1
		End
		Part	powers\EarthControl\StalagtiteCrack8.part
		Geom	VolcanicRock04
	End
End

#############################################################

End