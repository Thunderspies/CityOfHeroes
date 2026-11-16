#############################################################
## WeakenContinuing.fx
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
		Part	:GlowTrailBlackContfalling.part
		Part	:GlowTrailLightContfalling.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2falling.part
		Part	:GlowTrailBlackCont2falling.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
	End






	Event
		EName	3
		Type	Local
		At	ULEGL
		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:GlowTrailBlackContfalling.part
		Part	:GlowTrailLightContfalling.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2falling.part
		Part	:GlowTrailBlackCont2falling.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		PMagnet Knee1

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:GlowTrailBlackContfalling.part
		Part	:GlowTrailLightContfalling.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2falling.part
		Part	:GlowTrailBlackCont2falling.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		PMagnet FootL
	End

	Event
		EName	5
		Type	Local
		At	FootL

		BHVR	Behaviors/GenericParticleFade.Bhvr

		Part	:GlowTrailBlackContfalling.part
		Part	:GlowTrailLightContfalling.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2falling.part
		Part	:GlowTrailBlackCont2falling.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:GlowTrailBlackContfalling.part
		Part	:GlowTrailLightContfalling.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2falling.part
		Part	:GlowTrailBlackCont2falling.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		PMagnet Kneer

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:GlowTrailBlackContfalling.part
		Part	:GlowTrailLightContfalling.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2falling.part
		Part	:GlowTrailBlackCont2falling.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		PMagnet FootR
	End

###############################################################################

	Event
		EName	9
		Type	Local
		At	Hair
		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:GlowTrailBlackContfalling.part
		Part	:GlowTrailLightContfalling.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2falling.part
		Part	:GlowTrailBlackCont2falling.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:GlowTrailBlackContfalling.part
		Part	:GlowTrailLightContfalling.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2falling.part
		Part	:GlowTrailBlackCont2falling.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		PMagnet ElbowL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:GlowTrailBlackContfalling.part
		Part	:GlowTrailLightContfalling.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2falling.part
		Part	:GlowTrailBlackCont2falling.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:GlowTrailBlackContfalling.part
		Part	:GlowTrailLightContfalling.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2falling.part
		Part	:GlowTrailBlackCont2falling.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part

	End






	Event
		EName	13
		Type	Local
		At	UArmR
		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:GlowTrailBlackContfalling.part
		Part	:GlowTrailLightContfalling.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2falling.part
		Part	:GlowTrailBlackCont2falling.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		PMagnet ElbowL

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:GlowTrailBlackContfalling.part
		Part	:GlowTrailLightContfalling.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2falling.part
		Part	:GlowTrailBlackCont2falling.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	WepR
		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:GlowTrailBlackContfalling.part
		Part	:GlowTrailLightContfalling.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2falling.part
		Part	:GlowTrailBlackCont2falling.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part

	End


End

################################################################################################################

End