#############################################################
## StealPowerContinuing.fx
#############################################################

FxInfo

###################################################################

Input
	Inpname	Hips
End

###################################################################

Condition
	On	Time
	Time	40

	Event
		Type	Local
		At	hips
		Sound forcefield5_loop 70 70 .6
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 420
	End
End


Condition
	On	Time
	Time	40

	Event
		EName	1
		Type	Local
		At	Hips
		part	:HighLight01.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	2
		Type	Local
		At	FootR
		part	:HighLightBodyGlows.part
		part	:HighLightBodyGlowsFlat.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	3
		Type	Local
		At	ULEGL
		part	:HighLightMotionGlows.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet LLEGL

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part	:HighLightMotionGlows.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet FootL
	End

	Event
		EName	5
		Type	Local
		At	FootL
		part	:HighLightBodyGlows.part
		part	:HighLightBodyGlowsFlat.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part	:HighLightMotionGlows.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet LlegR

	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part	:HighLightMotionGlows.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet FootR
	End

	Event
		EName	9
		Type	Local
		At	Hair
		part	:HighLightHeadGlows.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	10
		Type	Local
		At	UArmL
		part	:HighLightArmMotionGlows.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet LArmL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part	:HighLightArmMotionGlows.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet WepL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part	:HighLightArmGlows.part
		Bhvr	Behaviors/GenericParticleFade.bhvr

	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part	:HighLightArmMotionGlows.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet LArmR

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part	:HighLightArmMotionGlows.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet WepR
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part	:HighLightArmGlows.part
		Bhvr	Behaviors/GenericParticleFade.bhvr

	End

End

End