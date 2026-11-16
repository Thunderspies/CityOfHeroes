#############################################################
## Mercenaries_Gunfire_GernadeLauncher.fx
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
		EName 	tracer
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
		bhvr	behaviors\Mercenaries_MorterLobbprojectile2.bhvr
		geom	grenade
		Part1	:BeanBagDust2.part
		Magnet	T_Root
		LookAt	T_Root
	End
End

##################################

Condition
	On 	PrimeHit

	Event
		EName 	Fire
		Type	Posit
		At	T_Root
		Part1	:FireExplosion.part
		Part2	powers\firecontrol\FireWeaponHitSpark.part
		Lifespan 2
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Root
		Part1	:GernadeLauncherSparks.part
		Sound grexplo2 120.0 120.0 1.0
		Lifespan 5
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Root
		Part	:FireExplosionFlash.part
		Sound grexplo2 120.0 120.0 1.0
		Lifespan 5
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	T_Root

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		300
			PhysForcePowerJitter	50
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