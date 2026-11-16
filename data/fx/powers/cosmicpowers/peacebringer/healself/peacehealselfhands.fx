#########################################################
##	chill touch hit
FxInfo

##################################
LifeSpan	70
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
		sound	peaceeyes1 60 50 .75
		LifeSpan	70
	End

	Event
		Type	Local 
		At	Primea
		 AltPiv 1
		BHVR	Behaviors/GenericparticleFade.bhvr
		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\CircleOfThornsEyes_21Green.part
		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\EyesMistLeftBLUE.part
		Sound peacehands1 70 70 .8
	
		LifeSpan	70
	End
	
	Event
		Type	Local 
		At	Primea
		 AltPiv 2
		BHVR	Behaviors/GenericparticleFade.bhvr
		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\CircleOfThornsEyes_21Green.part
		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\EyesMistRightBLUE.part
		LifeSpan	70
	End

End

Condition
	On	Time
	Time	15

	Event
		EName	1
		Type	Local
		At	WepR
		part1	:HealingHandGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		LifeSpan	47
	End

	Event
		EName	1
		Type	Local
		At	WepL
		part1	:HealingHandGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		LifeSpan	47
	End

End


End				