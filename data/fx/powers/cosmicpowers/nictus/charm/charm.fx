#########################################################
##	chill touch hit
FxInfo

############################################################
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

	Event
		EName 	Orb2
		Type	Local 
		At	UarmR
		part	:UpperTendrilBlue.part
		part	:UpperTendrilPurple.part
		part	:UpperTendrilBlack.part
		PMagnet	LarmR
	End

	Event
		EName 	Orb2
		Type	Local 
		At	LarmR
		part	:TendrilBlue.part
		part	:TendrilPurple.part
		part	:TendrilBlack.part
		PMagnet	WepR
	End

	Event
		EName 	Orb2
		Type	Local 
		At	WepR
		part	:UpperTendrilBlue.part
		part	:UpperTendrilPurple.part
		part	:UpperTendrilBlack.part
		PMagnet	target
	End

	Event
		EName 	Orb2
		Type	Local 
		At	WepR
		Part	:CharmBlack.part
		Part	:CharmBlue.part
		Part	:CharmPurple.part

	End
End

End			