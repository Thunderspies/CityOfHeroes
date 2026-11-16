#############################################################
## alcaloideHeal.fx
#############################################################

FxInfo
LifeSpan	60

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition

	Event
		EName	2
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont2.part
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont2.part
		Part	CustomizeablePowers\Poison\GlowstarsTrailCont2.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont2.part
		LifeSpan	40
	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont2.part
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont2.part
		Part	CustomizeablePowers\Poison\GlowstarsTrailCont2.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont2.part
		PMagnet Knee1
		LifeSpan	40

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont2.part
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont2.part
		Part	CustomizeablePowers\Poison\GlowstarsTrailCont2.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont2.part
		PMagnet FootL
		LifeSpan	40
	End

	Event
		EName	5
		Type	Local
		At	FootL

		Bhvr	Behaviors\GenericParticleFade.bhvr

		Part	CustomizeablePowers\Poison\GlowTrailBlackCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont2.part
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont2.part
		Part	CustomizeablePowers\Poison\GlowstarsTrailCont2.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont2.part
		LifeSpan	40
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont2.part
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont2.part
		Part	CustomizeablePowers\Poison\GlowstarsTrailCont2.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont2.part
		PMagnet Kneer
		LifeSpan	40

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont2.part
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont2.part
		Part	CustomizeablePowers\Poison\GlowstarsTrailCont2.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont2.part
		PMagnet FootR
		LifeSpan	40
	End

###############################################################################

	Event
		EName	9
		Type	Local
		At	Hair
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont2.part
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont2.part
		Part	CustomizeablePowers\Poison\GlowstarsTrailCont2.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont2.part
		LifeSpan	40
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont2.part
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont2.part
		Part	CustomizeablePowers\Poison\GlowstarsTrailCont2.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont2.part
		PMagnet ElbowL
		LifeSpan	40

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont2.part
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont2.part
		Part	CustomizeablePowers\Poison\GlowstarsTrailCont2.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont2.part
		PMagnet HandL
		LifeSpan	40
	End

	Event
		EName	12
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont2.part
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont2.part
		Part	CustomizeablePowers\Poison\GlowstarsTrailCont2.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont2.part
		LifeSpan	40

	End






	Event
		EName	13
		Type	Local
		At	UArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont2.part
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont2.part
		Part	CustomizeablePowers\Poison\GlowstarsTrailCont2.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont2.part
		PMagnet ElbowL
		LifeSpan	40

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont2.part
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont2.part
		Part	CustomizeablePowers\Poison\GlowstarsTrailCont2.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont2.part
		PMagnet HandL
		LifeSpan	40
	End

	Event
		EName	15
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont2.part
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont2.part
		Part	CustomizeablePowers\Poison\GlowstarsTrailCont2.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont2.part
		LifeSpan	40

	End


End

#################################################################################################################

Condition
	On	Time
	Time	15

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	CustomizeablePowers\Poison\StaminaRingUp.part
		Sound heal1 70 70 .8

	End

End

Condition
	On	Time
	Time	20

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	CustomizeablePowers\Poison\StaminaRingUp.part

	End

End

Condition
	On	Time
	Time	35

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	CustomizeablePowers\Poison\StaminaRingUp.part

	End

End

Condition
	On	Time
	Time	18

	Event
		EName	1
		Type	Local
		At	Hips
		part1	CustomizeablePowers\Poison\Healing01.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End

	Event
		EName	2
		Type	Local
		At	FootR
		part	CustomizeablePowers\Poison\HealEnergy.part
		part	CustomizeablePowers\Poison\HealEnergy.part
		part	CustomizeablePowers\Poison\Heal.part
	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		part	CustomizeablePowers\Poison\HealEnergy.part
		part	CustomizeablePowers\Poison\HealEnergy.part
		part	CustomizeablePowers\Poison\Heal.part
		PMagnet Knee1

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part	CustomizeablePowers\Poison\HealEnergy.part
		part	CustomizeablePowers\Poison\HealEnergy.part
		part	CustomizeablePowers\Poison\Heal.part
		PMagnet FootL
	End

	Event
		EName	5
		Type	Local
		At	FootL
		part	CustomizeablePowers\Poison\HealEnergy.part
		part	CustomizeablePowers\Poison\HealEnergy.part
		part	CustomizeablePowers\Poison\Heal.part
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part	CustomizeablePowers\Poison\HealEnergy.part
		part	CustomizeablePowers\Poison\HealEnergy.part
		part	CustomizeablePowers\Poison\Heal.part
		PMagnet Kneer

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part	CustomizeablePowers\Poison\HealEnergy.part
		part	CustomizeablePowers\Poison\HealEnergy.part
		part	CustomizeablePowers\Poison\Heal.part
		PMagnet FootR
	End

###############################################################################

	Event
		EName	9
		Type	Local
		At	Hair
		part	CustomizeablePowers\Poison\HealEnergy.part
		part	CustomizeablePowers\Poison\HealEnergy.part
		part	CustomizeablePowers\Poison\Heal.part
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part	CustomizeablePowers\Poison\HealEnergy.part
		part	CustomizeablePowers\Poison\HealEnergy.part
		part	CustomizeablePowers\Poison\Heal.part
		PMagnet ElbowL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part	CustomizeablePowers\Poison\HealEnergy.part
		part	CustomizeablePowers\Poison\HealEnergy.part
		part	CustomizeablePowers\Poison\Heal.part
		PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part	CustomizeablePowers\Poison\HealEnergy.part
		part	CustomizeablePowers\Poison\HealEnergy.part
		part	CustomizeablePowers\Poison\Heal.part

	End






	Event
		EName	13
		Type	Local
		At	UArmR
		part	CustomizeablePowers\Poison\HealEnergy.part
		part	CustomizeablePowers\Poison\HealEnergy.part
		part	CustomizeablePowers\Poison\Heal.part
		PMagnet ElbowL

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part	CustomizeablePowers\Poison\HealEnergy.part
		part	CustomizeablePowers\Poison\HealEnergy.part
		part	CustomizeablePowers\Poison\Heal.part
		PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part	CustomizeablePowers\Poison\HealEnergy.part
		part	CustomizeablePowers\Poison\HealEnergy.part
		part	CustomizeablePowers\Poison\Heal.part

	End



End

End