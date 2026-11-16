#########################################################
##	
##

FxInfo
LifeSpan  150
##################################


Condition
	On	Time
	Time	10

	Event
		Type	Local	
		At	Origin
		Sound grtoss3 80 80 .7
	End
End



Condition
	On 	Time
	Time 	10

	Event
		EName 	Prime1
		Type	local 
		At	wepR
		geom	GEO_WepR_Dynamite_stick
		bhvr	:DynamiteFade.bhvr
		lifespan	25
	End


	Event
		EName 	Fuse
		Type	Local
		At	Prime1		
		bhvr	:DynamiteFuseOffset.bhvr
		Part	:DynamiteFuseGlow.part	
		Part	:DynamiteFuseSparks.part
	End


End

Condition
	On 	Time
	Time 	36

	Event
		EName 	Prime
		Type	start 
		At	wepR
		geom	GEO_WepR_Dynamite_stick
		bhvr	:DynamiteProjectile.bhvr
		Magnet	T_Root
		LookAt	T_Root

	End

	Event
		EName 	Fuse
		Type	Local
		At	Prime		
		bhvr	:DynamiteFuseOffset.bhvr
		Part	:DynamiteFuseGlow.part	
		Part	:DynamiteFuseSparks.part			
		Part	:DynamiteFuseSparks.part
	End


End


##################################

Condition
	On 	PrimeHit

	Event
		EName 	Fire
		Type	Posit
		At	T_Root
		Part1	:DynamiteExplodeSmoke1.part
		Part1	:DynamiteExplodeSmoke2.part
		Part2	powers/firecontrol/FireWeaponHitSpark.part
		Lifespan 10
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Root
		Part1	WEAPONFX/GernadeLauncherSparks.part
		Part2	:DynamiteExplodeSparks.part
		Sound grexplo2 120.0 120.0 1.0
		Lifespan 5
		
	End

	Event
		EName 	Flash
		Type	Posit
		At	T_Root
		Part	WEAPONFX/FireExplosionFlash.part		
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

		Lifespan	1

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
	
	Event
		Ename 	Prime
		Type	Destroy
	End


End


End							