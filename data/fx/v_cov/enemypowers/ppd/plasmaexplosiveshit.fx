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
		#Part	:PlasmaBlastFlash.part
		#Part	:PlasmaBlastStar.part		
		Part	:PlasmaBlastHead.part

		Part	:LaserHitGlow1.part

		Part	:PlasmaHitGlowStreaks.part
		
		Part	:PlasmaFire.part

		Part	:PlasmaHitSparks.part

		Lifespan		10
	End
	
	Event
		Type	Local
		At	Chest

		BHVR	Behaviors/GenericParticleFade.Bhvr

		Part	:LaserHitGlow1.part
		Part	:PlasmaBlastHead.part
		Part	:PlasmaFire.part

		Lifespan		30
	End
		
	Event
		Type	Local
		At	Chest

		BHVR	Behaviors/GenericParticleFade.Bhvr

		Part	:PlasmaFire.part

		Lifespan		60
	End
	
End

##################################


End				