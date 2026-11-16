#############################################################
## Mercenary Mastermind Assault Rifle - Grenade Launcher
#############################################################

FxInfo

LifeSpan 200

## FIRE FX ###########################################################

Condition
	On 	Time
	Time 	30

	Event
		EName 	Blast
		Type	Start
		At	C_Chest
		Geom	Fx_GunBurst
		Lifespan 2
	End

	Event
		EName 	tracer
		Type	start
		At	C_Chest
		Part1	WEAPONFX\Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX\Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX\Gunfire_ShotgunMuzzflashSmall.part
		Sound Mercenary_Grenade 120.0 120.0 1.0
		Lifespan 12
	End
End

## PROJECTILE ###########################################################

Condition
	On 	Time
	Time 	30

	Event
		EName 	Prime
		Type	start
		At	C_Chest
		bhvr	behaviors\MorterLobbprojectile2.bhvr
		geom	grenade
		Part1	:BeanBagDust2.part
		Magnet	T_Root
		LookAt	T_Root
	End
End

## HIT FX ###########################################################

Condition
	On 	PrimeHit

	Event
		Type	Start
		At	T_Root
		BHVR	Behaviors\CameraShake01Subtle.bhvr
	End

	Event
		EName 	Fire
		Type	Posit
		At	T_Root
		Part1	:FireExplosion.part
		Lifespan 8
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
			PhysForceRadius		20
			PhysForcePower		500
			PhysForcePowerJitter	50
		End
		Lifespan 1
	End

	Event
		Ename 	Prime
		Type	Destroy
	End
End

Condition
	On	Primehit
	Repeat	24

	Event
		EName	Sparkout
		Type 	Start
		At	T_Root
		BhvrOverride
			PositionOffset	0 1.5 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
	End

	Event
		HardwareOnly
		EName	Sparkout
		Type 	Start
		At	T_Root
		BhvrOverride
			PositionOffset	0 1.5 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
	End
End

#############################################################

End