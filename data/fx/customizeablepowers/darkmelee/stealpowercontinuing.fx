#############################################################
## StealPowerContinuing.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	40

	Event
		Type	Local
		At	hips
		bhvr	behaviors\soundFade3.bhvr
		Sound forcefield5_loop 70 70 0.6
		Flags	PowerLoopingSound
		Lifespan 420
	End
End

Condition
	On	Time
	Time	40

	Event
		Type	Local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:HighLight01.part
	End
End

Condition
	On	Time
	Time	40

	Event
		Type	Local
		At	Hair
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:HighLightHeadGlows.part
	End

	Event
		Type	Local
		At	UArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:HighLightArmMotionGlows.part
		PMagnet LArmL
	End

	Event
		Type	Local
		At	LArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:HighLightArmMotionGlows.part
		PMagnet WepL
	End

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:HighLightArmGlows.part
	End

	Event
		Type	Local
		At	UArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:HighLightArmMotionGlows.part
		PMagnet LArmR
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:HighLightArmMotionGlows.part
		PMagnet WepR
	End

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:HighLightArmGlows.part
	End

	Event
		Type	Local
		At	ULEGL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:HighLightMotionGlows.part
		PMagnet LLEGL
	End

	Event
		Type	Local
		At	LLEGL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:HighLightMotionGlows.part
		PMagnet FootL
	End

	Event
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:HighLightBodyGlows.part
		Part	:HighLightBodyGlowsFlat.part
	End

	Event
		Type	Local
		At	ULEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:HighLightMotionGlows.part
		PMagnet LlegR
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:HighLightMotionGlows.part
		PMagnet FootR
	End

	Event
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:HighLightBodyGlows.part
		Part	:HighLightBodyGlowsFlat.part
	End
End

#############################################################

End