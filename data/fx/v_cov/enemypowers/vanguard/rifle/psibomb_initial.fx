#########################################################
## Vanguard "Redding" Rifle - Psi-Bomb (Initial Delayed)
#########################################################

FxInfo
LifeSpan  300

## FIRE #######################################################

Condition
	On 	Time
	Time 	42

	Event
		ENAME   AltPivots
		Type	Local 
		At	WepR
		Geom	Vanguard_Rifle
	End

	Event
		EName 	Prime
		Type	Start 
		At	AltPivots
		Altpiv	1
		geom	grenade
		bhvr	behaviors\MorterLobbprojectile2.bhvr
		Part1	WEAPONFX\BeanBagDust2.part
		Magnet	T_Root
		LookAt	T_Root
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
		Type	Start
		At	AltPivots
		Altpiv 1
		BHVR	WEAPONs\WeaponRotate.bhvr
		Geom	Fx_GunBurst
		Lifespan	1
	End
	
	Event
		EName 	tracer
		Type	Start
		At	AltPivots
		Altpiv	1
		BHVR	WEAPONs\WeaponRotate.bhvr
		Part1	WEAPONFX\Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX\Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX\Gunfire_ShotgunMuzzflashSmall.part
		Lifespan	2
	End
End

## ON HIT #######################################################

Condition
	On 	PrimeHit
	
	Event
		Type	Start
		At	Target
		BHVR	Behaviors\CameraShake01Subtle.bhvr
	End

	Event
		Type	Start 
		At	Target
		Sound Nova 100.0 100.0 .8
	End

	Event
		Type	Start 
		At	Target
		ChildFX	:PsiBomb\PsiBomb_Explosion.fx
	End
End

## CLEAN UP #######################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End
End

## PHYSICS FORCES #######################################################

Condition
	On 	PrimeHit

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Root
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		35
			PhysForcePower		1500
			PhysForcePowerJitter	200
		End
		Lifespan	1
	End

	Event
		Ename	GravitationalForce
		Type	Local
		At	Head
		BhvrOverride
			PhysForceType		In
			PhysForceRadius		20
			PhysForcePower		25
			PhysForcePowerJitter	10
		End
		Lifespan	50
	End
End

#########################################################

End