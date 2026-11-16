#########################################################
## Vanguard "Redding" Rifle - Willie Pete
#########################################################

FxInfo
LifeSpan  200

## FIRE #######################################################

Condition
	On 	Time
	Time 	33

	Event
		ENAME   AltPivots
		Type	Local 
		At	WepR
		Geom	Vanguard_Rifle
	End

	Event
		EName 	Prime
		Type	start 
		At	AltPivots
		Altpiv	1
		geom	grenade
		bhvr	behaviors\MorterLobbprojectile2.bhvr
		Part1	WEAPONFX\BeanBagDust2.part
		Magnet	T_Chest
		LookAt	T_Chest
	End

	Event
		EName 	Blast
		Type	Start
		At	AltPivots
		Altpiv	1
		Sound Mercenary_Grenade 120.0 120.0 1.0
		#LifeSpan	3
	End

	Event
		EName 	MuzzleFlash
		Type	start
		At	AltPivots
		Altpiv 1
		BHVR	WEAPONs\WeaponRotate.bhvr
		Geom	Fx_GunBurst
		Lifespan	2
	End
	
	Event
		EName 	tracer
		Type	start
		At	AltPivots
		Altpiv 1
		BHVR	WEAPONs\WeaponRotate.bhvr
		Part1	WEAPONFX\Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX\Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX\Gunfire_ShotgunMuzzflashSmall.part
		Lifespan	5
	End
End

## ON HIT #######################################################

Condition
	On 	PrimeHit
	
	Event
		Type	Start
		At	T_Root
		BHVR	Behaviors\CameraShake01Subtle.bhvr
	End
End

Condition
	On 	PrimeHit
	
	Event
		Ename	FireBallHit
		Type	local
		At	target
		Part1	POWERS\FireControl\BigFireExplosion.part
		Part2	POWERS\FireControl\FireBallHitSpark.part
	End

	Event
		EName 	Ringnode
		Type	Local 
		At	T_Root
		Geom	RootToChestAdjustment
		Sound Atomicblast 100 100 .9
		Sound FireExplo 100.0 100.0 .6
	End

	Event
		EName 	Rings
		Type	start 
		At	Ringnode
		altpiv	1
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		Part1	powers/firecontrol/FireBallHitRing1.part
		Part2	powers/firecontrol/FireBallHitRing2.part
		
	End

	Event
		EName 	Rocks
		Type	Local 
		At	T_Chest
		Part1	Generic\InfernorockBits.part
		Part2	Generic\InfernorockBitz.part
		Lifespan	5
	End

	Event 
		HardwareOnly
		EName 	Sparkz
		Type 	Local
		At 	T_hips
		ChildFX V_COV\PhysicsEnabled\FireBurstComet.fx
		Lifespan	70
		LifespanJitter 	20
	End
End

Condition
	On 	primehit
	Repeat  8

	Event 
		EName 	Sparkz
		Type 	Local
		At 	T_hips
		ChildFX V_COV\PhysicsEnabled\FireBurstExplode.fx
		Lifespan	70
		LifespanJitter 	20
	End

	Event 
		HardwareOnly
		EName 	Sparkz
		Type 	Local
		At 	T_hips
		ChildFX V_COV\PhysicsEnabled\FireBurstUP.fx
		Lifespan	70
		LifespanJitter 	20
	End
	
	Event 
		HardwareOnly
		EName 	Sparkz
		Type 	Local
		At 	T_Chest
		ChildFX V_COV\PhysicsEnabled\FireBurstExplode.fx
		Lifespan	70
		LifespanJitter 	20
	End
End

## PHYSICS FORCES #######################################################

Condition
	On 	PrimeHit

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

## CLEAN UP #######################################################

Condition
	On 	PrimeHit
	
	Event
		Ename 	Prime
		Type	Destroy
	End
End

#########################################################

End