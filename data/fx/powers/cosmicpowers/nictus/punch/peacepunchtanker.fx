#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	50
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
		Sound Warshadeeyes3 60 60 .72
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
	Time 	0

	Event
		EName	SwordFlash2
		Type	Local
		At	WepR
		
		Part	:PunchTendrils.part
		Part	:PunchMistEnergy.part
		Part	:PunchTendrilsBlue.part
		Part	:PunchMistEnergyBlue.part
		Part	:PunchTendrilsPurple.part
		Part	:PunchMistEnergyPurple.part
		
		LifeSpan 25
	End

	Event
		EName	SwordFlash2
		Type	Local
		At	WepL
		
		Part	:PunchTendrils.part
		Part	:PunchMistEnergy.part
		Part	:PunchTendrilsBlue.part
		Part	:PunchMistEnergyBlue.part
		Part	:PunchTendrilsPurple.part
		Part	:PunchMistEnergyPurple.part
		
		LifeSpan 25
	End

End

End	


			