#########################################################
##	chill touch hit
FxInfo

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
		EName	2
		Type	Local
		At	FootR
		part1	:HighLightBodyGlows.part
		part2	:HighLightBodyGlowsFlat.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:HighLightMotionGlows.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet LLEGL

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:HighLightMotionGlows.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet FootL
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	:HighLightBodyGlows.part
		part2	:HighLightBodyGlowsFlat.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:HighLightMotionGlows.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet LlegR

	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:HighLightMotionGlows.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet FootR
	End

	Event
		EName	9
		Type	Local
		At	Head
		part2	:HighLightHeadGlows.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:HighLightArmMotionGlows.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet LArmL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:HighLightArmMotionGlows.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet WepL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	:HighLightArmGlows.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:HighLightArmMotionGlows.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet LArmR

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:HighLightArmMotionGlows.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet WepR
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	:HighLightArmGlows.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	
	End

End

End