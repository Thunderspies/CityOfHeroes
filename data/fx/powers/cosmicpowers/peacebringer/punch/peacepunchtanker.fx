#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	30
#################################################################
Condition
	On 	Time
	Time 	0

	Event
		EName 	Primea
		Type	Local 
		At	Eyes
		BHVR	Behaviors/OffsetEyes.bhvr
		Geom	CircleOfThorns
		sound	peaceeyes1 60 50 .7
		LifeSpan	30
	End

	Event
		Type	Local 
		At	Primea
		 AltPiv 1
		BHVR	Behaviors/GenericparticleFade.bhvr
		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\CircleOfThornsEyes_21Green.part
		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\EyesMistLeftBLUE.part
	
		LifeSpan	30
	End
	
	Event
		Type	Local 
		At	Primea
		 AltPiv 2
		BHVR	Behaviors/GenericparticleFade.bhvr
		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\CircleOfThornsEyes_21Green.part
		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\EyesMistRightBLUE.part
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
		At	wepR
		Part	:PunchFlash.part
		Part	:PunchTendrils.part
		Part	:PunchMistEnergy.part
		Sound peacepunch 80 80 .86
		LifeSpan 15
	End

	Event
		EName	SwordFlash2
		Type	Local
		At	wepL
		Part	:PunchFlash.part
		Part	:PunchTendrils.part
		Part	:PunchMistEnergy.part
		Sound peacepunch 80 80 .86
		LifeSpan 15
	End

End

End	


			