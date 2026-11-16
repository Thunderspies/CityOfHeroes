#########################################################
##	
##

FxInfo

LifeSpan  15

##################################


Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Chest

		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:PlasmaBlastFlash.part
		Part	:PlasmaBlastStar.part

		Part	:PlasmaHitSparks.part
		Part	:PlasmaHitSparks.part

		Part	:PlasmaExplosionStreaks.part
		Part	:PlasmaExplosionFire.part
		Part	:PlasmaExplosionFlash.part
		Part	:PlasmaExplosionRing.part
		Part	:PlasmaExplosionRingFlat.part

		LifeSpan	10
	End
	
	Event
		Type	Start
		At	Chest

		BHVR	Behaviors/GenericParticleFade.Bhvr
		
		Part	:PlasmaBlastHead.part

		Part	:LaserHitGlow1.part

		Part	:PlasmaHitGlowStreaks.part
		Part	:PlasmaHitFire.part
		
		Part	:PlasmaFire.part
	End
	
	Event
		Type	Start
		At	Chest

		BHVR	Behaviors/CameraShake01Subtle.bhvr
		
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		400
			PhysForcePowerJitter	100
		End

		Lifespan	1

	End
End

##################################


End				