#############################################################
## Arachnos Soldier - Crab Spider - Web Grenade
#############################################################

FxInfo

LifeSpan  200

Flags	InheritAnimScale

## FIRE ###########################################################

Condition
	On 	Time
	Time 	35

	Event
		Type	start
		At	C_Fore_ToeL
		Part1	WEAPONFX\Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX\Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX\Gunfire_ShotgunMuzzflashSmall.part
		sound	grenade2 90.0 30.0 .7
		Lifespan 15
	End
End

## PROJECTILE ###########################################################

Condition
	On 	Time
	Time 	35

	Event
		EName 	Prime
		Type	start
		At	C_Fore_ToeL
		geom	grenade
		bhvr	behaviors\MorterLobbprojectile2.bhvr
		Part1	WeaponFx\BeanBagDust2.part
		Magnet	T_Root
		LookAt	T_Root
	End
End

## ON PRIME HIT ###########################################################

Condition
	On 	PrimeHit

	Event
		Ename 	Prime
		Type	Destroy
	End

	Event
		Type	start
		At	T_Root
		ChildFx	V_COV\EnemyPowers\Arachnos\GrenadeLauncher\FragGrenadeHit.fx
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	T_Root

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		500
			PhysForcePowerJitter	50
		End
		Lifespan 1
	End
End

#############################################################

End