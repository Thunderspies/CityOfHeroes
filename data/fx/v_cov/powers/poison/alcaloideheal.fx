#############################################################
## alcaloideHeal.fx
#############################################################

FxInfo
LifeSpan	400

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
		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:GlowTrailBlackCont.part
		Part	:GlowTrailLightCont.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2.part
		Part	:GlowTrailBlackCont2.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		LifeSpan	40
	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:GlowTrailBlackCont.part
		Part	:GlowTrailLightCont.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2.part
		Part	:GlowTrailBlackCont2.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		PMagnet Knee1
		LifeSpan	40

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:GlowTrailBlackCont.part
		Part	:GlowTrailLightCont.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2.part
		Part	:GlowTrailBlackCont2.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		PMagnet FootL
		LifeSpan	40
	End

	Event
		EName	5
		Type	Local
		At	FootL

		BHVR	Behaviors/GenericParticleFade.Bhvr

		Part	:GlowTrailBlackCont.part
		Part	:GlowTrailLightCont.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2.part
		Part	:GlowTrailBlackCont2.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		LifeSpan	40
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:GlowTrailBlackCont.part
		Part	:GlowTrailLightCont.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2.part
		Part	:GlowTrailBlackCont2.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		PMagnet Kneer
		LifeSpan	40

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:GlowTrailBlackCont.part
		Part	:GlowTrailLightCont.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2.part
		Part	:GlowTrailBlackCont2.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		PMagnet FootR
		LifeSpan	40
	End

###############################################################################

	Event
		EName	9
		Type	Local
		At	Hair
		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:GlowTrailBlackCont.part
		Part	:GlowTrailLightCont.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2.part
		Part	:GlowTrailBlackCont2.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		LifeSpan	40
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:GlowTrailBlackCont.part
		Part	:GlowTrailLightCont.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2.part
		Part	:GlowTrailBlackCont2.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		PMagnet ElbowL
		LifeSpan	40

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:GlowTrailBlackCont.part
		Part	:GlowTrailLightCont.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2.part
		Part	:GlowTrailBlackCont2.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		PMagnet HandL
		LifeSpan	40
	End

	Event
		EName	12
		Type	Local
		At	WepL
		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:GlowTrailBlackCont.part
		Part	:GlowTrailLightCont.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2.part
		Part	:GlowTrailBlackCont2.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		LifeSpan	40

	End






	Event
		EName	13
		Type	Local
		At	UArmR
		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:GlowTrailBlackCont.part
		Part	:GlowTrailLightCont.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2.part
		Part	:GlowTrailBlackCont2.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		PMagnet ElbowL
		LifeSpan	40

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:GlowTrailBlackCont.part
		Part	:GlowTrailLightCont.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2.part
		Part	:GlowTrailBlackCont2.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		PMagnet HandL
		LifeSpan	40
	End

	Event
		EName	15
		Type	Local
		At	WepR
		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:GlowTrailBlackCont.part
		Part	:GlowTrailLightCont.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2.part
		Part	:GlowTrailBlackCont2.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
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
		part1	Powers/Ability/StaminaRingUp.part
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
		part1	Powers/Ability/StaminaRingUp.part

	End

End

Condition
	On	Time
	Time	35

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	Powers/Ability/StaminaRingUp.part

	End

End

Condition
	On	Time
	Time	18

	Event
		EName	1
		Type	Local
		At	Hips
		part1	Powers/Healing/Healing01.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	2
		Type	Local
		At	FootR
		part	V_COV\Powers\Heal\HealEnergy.part
		part	V_COV\Powers\Heal\HealEnergy.part
		part	V_COV\Powers\Heal\Heal.part
	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		part	V_COV\Powers\Heal\HealEnergy.part
		part	V_COV\Powers\Heal\HealEnergy.part
		part	V_COV\Powers\Heal\Heal.part
		PMagnet Knee1

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part	V_COV\Powers\Heal\HealEnergy.part
		part	V_COV\Powers\Heal\HealEnergy.part
		part	V_COV\Powers\Heal\Heal.part
		PMagnet FootL
	End

	Event
		EName	5
		Type	Local
		At	FootL
		part	V_COV\Powers\Heal\HealEnergy.part
		part	V_COV\Powers\Heal\HealEnergy.part
		part	V_COV\Powers\Heal\Heal.part
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part	V_COV\Powers\Heal\HealEnergy.part
		part	V_COV\Powers\Heal\HealEnergy.part
		part	V_COV\Powers\Heal\Heal.part
		PMagnet Kneer

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part	V_COV\Powers\Heal\HealEnergy.part
		part	V_COV\Powers\Heal\HealEnergy.part
		part	V_COV\Powers\Heal\Heal.part
		PMagnet FootR
	End

###############################################################################

	Event
		EName	9
		Type	Local
		At	Hair
		part	V_COV\Powers\Heal\HealEnergy.part
		part	V_COV\Powers\Heal\HealEnergy.part
		part	V_COV\Powers\Heal\Heal.part
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part	V_COV\Powers\Heal\HealEnergy.part
		part	V_COV\Powers\Heal\HealEnergy.part
		part	V_COV\Powers\Heal\Heal.part
		PMagnet ElbowL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part	V_COV\Powers\Heal\HealEnergy.part
		part	V_COV\Powers\Heal\HealEnergy.part
		part	V_COV\Powers\Heal\Heal.part
		PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part	V_COV\Powers\Heal\HealEnergy.part
		part	V_COV\Powers\Heal\HealEnergy.part
		part	V_COV\Powers\Heal\Heal.part

	End






	Event
		EName	13
		Type	Local
		At	UArmR
		part	V_COV\Powers\Heal\HealEnergy.part
		part	V_COV\Powers\Heal\HealEnergy.part
		part	V_COV\Powers\Heal\Heal.part
		PMagnet ElbowL

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part	V_COV\Powers\Heal\HealEnergy.part
		part	V_COV\Powers\Heal\HealEnergy.part
		part	V_COV\Powers\Heal\Heal.part
		PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part	V_COV\Powers\Heal\HealEnergy.part
		part	V_COV\Powers\Heal\HealEnergy.part
		part	V_COV\Powers\Heal\Heal.part

	End



End



Condition
	On	Time
	Time	58

	Event
		EName	1
		Type	Destroy

	End

	Event
		EName	2
		Type	Destroy

	End

	Event
		EName	3
		Type	Destroy

	End

	Event
		EName	4
		Type	Destroy

	End

	Event
		EName	5
		Type	Destroy

	End

	Event
		EName	6
		Type	Destroy

	End

	Event
		EName	7
		Type	Destroy

	End

	Event
		EName	9
		Type	Destroy

	End

	Event
		EName	10
		Type	Destroy

	End

	Event
		EName	11
		Type	Destroy

	End

	Event
		EName	12
		Type	Destroy

	End

	Event
		EName	13
		Type	Destroy

	End

	Event
		EName	14
		Type	Destroy

	End

	Event
		EName	15
		Type	Destroy

	End

End

Condition
	On	Time
	Time	60

	Event
		EName	all
		Type	Destroy

	End

End


End