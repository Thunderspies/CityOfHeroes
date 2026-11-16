#############################################################
## GrenadeThrow_Smoke.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Origin
		Sound grtoss1 60 60 .7
	End
End

Condition
	On 	Time
	Time 	10

	Event
		Type	local
		At	wepR
		geom	grenade
		bhvr	behaviors\grenadeFade.bhvr
		lifespan 22
	End
End

#############################################################

Condition
	On 	Time
	Time 	32

	Event
		EName 	Prime
		Type	start
		At	wepR
		bhvr	behaviors\MorterLobbprojectile2.bhvr
		geom	grenade
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
		Part1	V_COV\WeaponFx\DynamiteExplodeSmoke1.part
		Part1	V_COV\WeaponFx\DynamiteExplodeSmoke2.part
		Part	V_COV\WeaponFx\SmokeScreen.part
		Lifespan 60
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Root
		#Part1	:GernadeLauncherSparks.part
		#Part2	:GernadeLauncherSpark.part
		Sound grexplo2 120.0 120.0 1.0
		Lifespan 5
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Root
		Part	:FireExplosionFlash.part
		Part2	V_COV\WeaponFx\DynamiteExplodeSparks.part
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
		Lifespan	1
	End

	Event
		Ename 	Prime
		Type	Destroy
	End
End

##################################

Condition
	On 	Time
	Time	34

	Event
		Ename 	Blast
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	43

	Event
		Ename 	tracer
		Type	Destroy
	End

End

Condition
	On 	PrimeHit



End

#############################################################

End