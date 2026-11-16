#############################################################
## Mercenaries_Gunfire_GernadeLauncher_Gas.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		EName 	weporigin
		Type	local
		At	WepR
		Geom	Mastermind_Military_Rifle
	End

	Event
		Type	start
		At	weporigin
		Altpiv	1
		BHVR	WEAPONs\WeaponRotate.bhvr
		Part1	WEAPONFX\SpecOpps_ShotgunMuzzflash.part
		Part2	WEAPONFX\SpecOpps_Shotgunsmoke.part
		Part3	WEAPONFX\SpecOpps_ShotgunMuzzflashSmall.part
		Sound Mercenary_Grenade 120.0 120.0 1.0
		Lifespan 12
	End
End

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		EName 	Prime
		Type	start
		At	weporigin
		Altpiv	1
		geom	grenade
		bhvr	behaviors\Mercenaries_MorterLobbprojectile2.bhvr
		Part1	:BeanBagDust2.part
		Magnet	T_Root
		LookAt	T_Root

	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		EName 	Smoke
		Type	Posit
		At	T_Root
		Part1	V_COV\WeaponFx\TearGasExplodeSmoke1.part
		Part1	V_COV\WeaponFx\TearGasExplodeSmoke2.part
		Part	POWERS\ItemsAndTempPowers\TearGas01.part
		Part1	POWERS\ItemsAndTempPowers\TearGasFlat.part
		Lifespan 60
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Root
		Part	:FireExplosionFlash.part
		Part2	V_COV\WeaponFx\DynamiteExplodeSparks.part
		Sound grexplo2 120.0 120.0 1.0
		Lifespan 5
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	T_Root
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		100
			PhysForcePowerJitter	10
		End
		Lifespan 1
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		Ename 	Prime
		Type	Destroy
	End
End

#############################################################

End