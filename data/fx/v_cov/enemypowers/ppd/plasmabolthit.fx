#########################################################
##	
##

FxInfo

LifeSpan  10

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
		Part	:PlasmaBlastHead.part

		Part	:LaserHitGlow1.part

		Part	:PlasmaHitGlowStreaks.part
		Part	:PlasmaHitFire.part
		
		Part	:PlasmaFire.part

		Part	:PlasmaHitSparks.part
		Sound legacyblasthit2 80 80 .88
	End
	
End

##################################


End				