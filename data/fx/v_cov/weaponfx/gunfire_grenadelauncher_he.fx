#############################################################
## Gunfire_GrenadeLauncher_HE.fx
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
		Geom	FlameThrower
	End

	Event
		EName 	Blast
		Type	Start
		At	weporigin
		Altpiv	1
		Geom	Fx_GunBurst
		Sound Grenade2 120.0 120.0 1.0
		Lifespan 2
	End

	Event
		EName 	tracer
		Type	start
		At	weporigin
		Altpiv	1
		Part1	WEAPONFX\Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX\Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX\Gunfire_ShotgunMuzzflashSmall.part
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
		bhvr	behaviors\MorterLobbprojectile2.bhvr
		Part1	WEAPONFX\BeanBagDust2.part
		Magnet	T_Root
		LookAt	T_Root

	End

End

##################################

Condition
	On 	PrimeHit

	Event
		Type	Start
		At	T_Root
		BHVR	Behaviors\CameraShake03Subtle.bhvr
	End

	Event
		EName 	Fire
		Type	Posit
		At	T_Root
		Part1	WEAPONFX\FireExplosion.part
		Part1	:DynamiteExplodeSmoke1.part
		Part1	:DynamiteExplodeSmoke2.part
		Part1   powers\firecontrol\mineflash.part
		Lifespan 8
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Root
		Part1	WEAPONFX\GernadeLauncherSparks.part
		Part2	:DynamiteExplodeSparks.part
		Part2	:DynamiteExplodeSparks.part
		Sound grexplo2 120.0 120.0 .8
		Lifespan 5
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Root
		Part	WEAPONFX\FireExplosionFlash.part
		Sound boom_explo3 120.0 120.0 1.0
		Lifespan 5
	End

	Event
		EName 	Ring
		Type	Posit
		At	T_Root
		BhvrOverride
			PositionOffset	0 2.5 0
		End
		Part1   :ExplosiveHitRingFlat.part
		Part1   :ExplosiveHitRing.part
		Lifespan 10
	End

	Event
		EName 	splatSpinsAndOffsets
		Type	start
		At	T_Root
		Geom	CenterDummy
		BHVR	:RandomSplatRotate.bhvr
	End

	Event
		EName 	scorchmark
		Type	start
		At	splatSpinsAndOffsets
		Altpiv	1

		BHVR	:RandomSplat.bhvr
		Splat	generic_cloud_02.tga #Dark_spark.tga #generic_cloud_02.tga
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	T_Root
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		1000
			PhysForcePowerJitter	100
		End
		Lifespan 1
	End
End

#############################################################

Condition
	On	Primehit
	Repeat	32

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

Condition
	On 	PrimeHit

	Event
		Ename 	Prime
		Type	Destroy
	End
End

#############################################################

End