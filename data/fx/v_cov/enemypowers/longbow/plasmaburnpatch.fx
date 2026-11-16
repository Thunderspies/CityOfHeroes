#########################################################
##	
##

FxInfo

LifeSpan  60

##################################


Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Chest

		BHVR	Behaviors/GenericParticleFade.Bhvr
		#Part	V_COV\EnemyPowers\PPD\PlasmaBlastFlash.part
		#Part	V_COV\EnemyPowers\PPD\PlasmaBlastStar.part		
		#Part	V_COV\EnemyPowers\PPD\PlasmaBlastHead.part
		#Part	V_COV\EnemyPowers\PPD\PlasmaHitGlowStreaks.part

		Part	V_COV\EnemyPowers\PPD\LaserHitGlow1.part
		Part	V_COV\EnemyPowers\PPD\PlasmaFire.part
		Part	V_COV\EnemyPowers\PPD\PlasmaHitSparks.part

		Lifespan		10
	End
	
	Event
		Type	Local
		At	Chest

		BHVR	Behaviors/GenericParticleFade.Bhvr

		Part	V_COV\EnemyPowers\PPD\PlasmaFire.part
		Part	V_COV\EnemyPowers\PPD\LaserHitGlow1.part
		Part	V_COV\EnemyPowers\PPD\PlasmaBlastHead.part

	End
	
End

##################################


End				