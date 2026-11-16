#############################################################
## Header
#############################################################

FxInfo

LifeSpan 100

#############################################################

Condition
	On 	Time
	Time 	3

	Event
		Type	start
		At	C_Chest
		Geom	FX_MachineGunBurst
		LifeSpan  1
	End

	Event
		EName 	Prime
		Type	start
		At	C_Chest
		Bhvr	Behaviors/Missleprojectile.bhvr
		Geom	_5th_missile
		Part1	AnimatedCharacterParts/5thmissile_smoke.part
		Part2	AnimatedCharacterParts/5thmissile_smoke2.part
		Part3	AnimatedCharacterParts/5thmissile_fire.part
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	Blast1
		Type	start
		At	C_Chest
		Part1	WEAPONFX/Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
		Sound rocketlaunch 100.0 100.0 .9
	End

	Event
		EName 	KickBackSmoke
		Type	start
		At	C_Chest
		Part1	:RocketLauncherSmoke.part
		Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
		Part4	WEAPONFX/Gunfire_MissleShell.Part
	End
End

#############################################################

Condition
	On	PrimeHit
	Repeat	7

	Event
		EName	Sparkup
		Type 	Start
		At	T_Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
	End
End

Condition
	On	PrimeHit
	Repeat	15

	Event
		HardwareOnly
		EName	Sparkup
		Type 	Start
		At	T_Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
	End
End

Condition
	On 	PrimeHit

	Event
		EName 	Fire
		Type	Posit
		At	T_Chest
		Part1	:FireExplosion.part
		#Part2	powers/firecontrol/FireWeaponHitSpark.part
		Lifespan 8
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Chest
		Part1	:GernadeLauncherSparks.part
		#Part2	:GernadeLauncherSpark.part
		Sound grexplo2 120.0 120.0 1.0
		Lifespan 5

	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Chest
		Part	:FireExplosionFlash.part
		#Part2	:GernadeLauncherSpark.part
		Lifespan 5

	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	T_Chest
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		500
			PhysForcePowerJitter	25
		End
		Lifespan	1
	End

	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		Ename	Explosion
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	6

	Event
		Ename 	Blast1
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	10

	Event
		Ename 	KickBackSmoke
		Type	Destroy
	End
End

#############################################################

End