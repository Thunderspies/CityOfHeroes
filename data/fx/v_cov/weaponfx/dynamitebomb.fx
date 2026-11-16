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
		Sound molotovtoss3 100 100 .7
	End
End



Condition
	On 	Time
	Time 	10

	Event
		EName 	Prime1
		Type	local 
		At	wepR
		geom	GEO_WepR_Time_Bomb
		bhvr	:DynamiteFade.bhvr		
		Part	:DynamiteBombGlow.part
		lifespan	22
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
	Time 	32

	Event
		EName 	Prime
		Type	start 
		At	wepR
		geom	GEO_WepR_Time_Bomb
		bhvr	:DynamiteProjectile.bhvr		
		Part	:DynamiteBombGlow.part
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
##    Explosion
##################################

Condition
	On 	PrimeHit

	Event
		EName 	Smoke
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
		Part2	:DynamiteExplodeSparks.part
		Sound molotovhit1 90.0 90.0 1.0
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
		EName 	Fire
		Type	Posit
		At	T_Root
		ChildFX	POWERS\FireControl\MineExplosion.fx		
		Lifespan 60
		
	End

#	Event
#		Ename	ExplosiveForce
#		Type	Start
#		At	T_Root
#		
#		BhvrOverride
#			PhysForceType		Out
#			PhysForceRadius		30
#			PhysForcePower		1000
#			PhysForcePowerJitter	100
#		End
#
#		Lifespan	1
#
#	End

	Event
		EName 	Fire2
		Type	Posit
		At	T_Root
		ChildFX	POWERS\FireControl\IndependencePortFire1.fx		
		Lifespan 120
		
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