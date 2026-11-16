#############################################################
## Arachnos Soldier - Wolf Spider - Frag Grenade
#############################################################

FxInfo

LifeSpan 200

## FIRE FX ###########################################################

Condition
	On 	Time
	Time 	7

	Event
		EName 	Blast
		Type	Start
		At	C_Chest
		Geom	Fx_GunBurst
		#Sound	Mercenary_Grenade 120.0 30.0 1.0
		sound	shotgun5 120.0 30.0 .9
		Lifespan	3
	End

	Event
		EName 	tracer
		Type	start
		At	C_Chest
		Part1	WEAPONFX\Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX\Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX\Gunfire_ShotgunMuzzflashSmall.part
		Lifespan	12
	End
End

## PROJECTILE ###########################################################

Condition
	On 	Time
	Time 	7

	Event
		EName 	Prime
		Type	start
		At	C_Chest
		bhvr	behaviors\MorterLobbprojectile2.bhvr
		geom	grenade
		Part1	WEAPONFX\BeanBagDust2.part
		Magnet	T_Root
		LookAt	T_Root
	End
End

## ON-HIT ###########################################################

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