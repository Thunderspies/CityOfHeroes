#############################################################
## Gunfire_GrenadeLauncher_Incendiary.fx
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
		geom	grenade
		bhvr	behaviors\MorterLobbprojectile2.bhvr
		Part1	WEAPONFX\BeanBagDust2.part
		Magnet	T_Root
		LookAt	T_Root
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		EName 	Fire
		Type	Posit
		At	T_Root
		Part1	WEAPONFX\FireExplosion.part
		Lifespan 8
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Root
		Part1	WEAPONFX\GernadeLauncherSparks.part
		#Part2	WEAPONFX\GernadeLauncherSpark.part
		Sound grexplo2 120.0 120.0 1.0
		Lifespan 5
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Root
		Part	WEAPONFX\FireExplosionFlash.part
		Part2	:DynamiteExplodeSparks.part
		Part2	:DynamiteExplodeSparks.part
		Lifespan 5
	End

	Event
		EName 	Fire
		Type	Posit
		At	T_Root
		BhvrOverride
			PositionOffset	0 1.5 0
		End
		ChildFX	explosions\fireexplosion\FireExplosionMedium.fx
		Lifespan 150
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	T_Root
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		25
			PhysForcePower		200
			PhysForcePowerJitter	25
		End
		Lifespan 1
	End
End

#############################################################

Condition
	On 	PrimeHit
	Repeat  11

	Event
		EName 	Sparkz
		Type 	Local
		At	T_Root
		BhvrOverride
			PositionOffset	0 1.5 0
		End
		ChildFX V_COV\PhysicsEnabled\FireBurstOUT.fx
		Lifespan 70
		LifespanJitter 20
	End
End

Condition
	On 	PrimeHit
	Repeat  7

	Event
		EName 	Sparkz
		Type 	Local
		At	T_Root
		BhvrOverride
			PositionOffset	0 1.5 0
		End
		ChildFX V_COV\PhysicsEnabled\FireBurstScatter.fx
		Lifespan 70
		LifespanJitter 	20
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