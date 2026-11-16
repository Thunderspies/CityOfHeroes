#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 300

#############################################################

Condition
	On 	Time
	Time	6

	Event
		Type	StartPositOnly
		At	Origin
		BHVR	Behaviors\GenericParticleFade.Bhvr
		Part	V_COV\EnemyPowers\PPD\PlasmaBlastFlash.part
		Part	V_COV\EnemyPowers\PPD\PlasmaBlastStar.part
		Part	V_COV\EnemyPowers\PPD\PlasmaHitSparks.part
		Part	V_COV\EnemyPowers\PPD\PlasmaHitSparks.part
		Part	V_COV\EnemyPowers\PPD\PlasmaExplosionStreaks.part
		Part	V_COV\EnemyPowers\PPD\PlasmaExplosionFire.part
		Part	V_COV\EnemyPowers\PPD\PlasmaExplosionFlash.part
		Part	V_COV\EnemyPowers\PPD\PlasmaExplosionRing.part
		Part	V_COV\EnemyPowers\PPD\PlasmaExplosionRingFlat.part
		LifeSpan 10
	End

	Event
		Type	StartPositOnly
		At	Origin
		BHVR	Behaviors\GenericParticleFade.Bhvr
		Part	V_COV\EnemyPowers\PPD\PlasmaBlastHead.part
		Part	V_COV\EnemyPowers\PPD\LaserHitGlow1.part
		Part	V_COV\EnemyPowers\PPD\PlasmaHitGlowStreaks.part
		Part	V_COV\EnemyPowers\PPD\PlasmaHitFire.part
		Part	V_COV\EnemyPowers\PPD\PlasmaFire.part
		ChildFX	:PlasmaCascade.fx
		LifeSpan 20
	End

	Event
		Type	StartPositOnly
		At	Origin
		ChildFX	:PlasmaCascade.fx
		LifeSpan 300
	End

	Event
		Type	StartPositOnly
		At	Origin
		BHVR	Behaviors\CameraShake01Subtle.bhvr
	End
End

#############################################################

End