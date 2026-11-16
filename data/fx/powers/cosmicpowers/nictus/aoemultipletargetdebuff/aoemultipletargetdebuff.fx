#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200
########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Primea
		Type	Local 
		At	Eyes
		BHVR	Behaviors/OffsetEyes.bhvr
		Geom	CircleOfThorns
		LifeSpan	30
	End

	Event
		Type	Local 
		At	Primea
		 AltPiv 1
		 BHVR	Behaviors/GenericParticleFade.bhvr
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyesMistLeftBLK.part
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyes_21Yellow.part
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyes_21Green.part
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\Eyes_BLUE.part
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\Eyes_PURPLE.part
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyes_BLACK.part
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyesMistLeft.part
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyesMistLeftBLUE.part
		LifeSpan	30
		
	End
	
	Event
		Type	Local 
		At	Primea
		 AltPiv 2
		 BHVR	Behaviors/GenericParticleFade.bhvr
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyesMistRightBLK.part
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyes_21Yellow.part
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyes_21Green.part
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\Eyes_BLUE.part
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\Eyes_PURPLE.part
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyes_BLACK.part
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyesMistRight.part
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyesMistRightBLUE.part
		LifeSpan	30
	End

End
######################################################################
Condition
	On 	Time
	Time 	16

	Event
		EName 	Prime
		Type	Start 
		At	T_WepR
		Bhvr	:MultipleSlowProjectile.bhvr
		Magnet	WepR
		LookAt	WepR
		Part	:BlackEnergyTrailOutline.part
		Part	:Sparks.part
		Part	:BlackEnergyTrailTendril.part
		Part	:SmallBlastHeadCore.part
		Part	:SmallBlastHeadCore.part
		Part	:SmallBlastHeadCore.part
		
	End
End

Condition
	On PrimeHit

	Event
		EName	SwordFlash2
		Type	Local
		At	WepR
		Part1	:PeaceHandsFlash.part
		Part	:QuickBlastHandStreaks.part
		Part	:QuickBlastHandGlow.part
		Part	:QuickBlastHandRing.part
		LifeSpan 5
	End

	Event
		Ename	Prime
		Type	Destroy
	End

End


End	


			