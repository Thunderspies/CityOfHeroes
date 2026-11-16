#############################################################
## Gunfire_GrenadeLauncher_TearGas.fx
#############################################################

FxInfo

LifeSpan 240

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	weporigin
		Type	local
		At	WepR
		Geom	FlameThrower
	End
End

Condition
	On 	Time
	Time 	7

	Event
		Type	Start
		At	weporigin
		Altpiv	1
		Geom	Fx_GunBurst
		Lifespan 2
	End

	Event
		Type	start
		At	weporigin
		Altpiv	1
		Part1	WEAPONFX\Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX\Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX\Gunfire_ShotgunMuzzflashSmall.part
		Sound Grenade2 120.0 120.0 1.0
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
		bhvr	behaviors\MorterLobbprojectile2.bhvr
		geom	grenade
		Part1	WEAPONFX\BeanBagDust2.part
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
		Part1	:TearGasExplodeSmoke1.part
		Part1	:TearGasExplodeSmoke2.part
		Part	POWERS\ItemsAndTempPowers\TearGas01.part
		Part1	POWERS\ItemsAndTempPowers\TearGasFlat.part
		Lifespan 60
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Root
		Part	WEAPONFX\FireExplosionFlash.part
		Part2	:DynamiteExplodeSparks.part
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