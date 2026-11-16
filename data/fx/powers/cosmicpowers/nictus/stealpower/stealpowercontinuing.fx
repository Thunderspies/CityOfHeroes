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
		Sound hydrapod2_loop 60 60 .22
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 480
	End
End


Condition
	On	Time
	Time	40

	Event
		EName	2
		Type	Local
		At	FootR
		part1	:HighLightBodyGlowsBLK.part
		part1	:HighLightBodyGlows.part
		part2	:HighLightBodyGlowsRed.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:HighLightMotionGlowsBLK.part
		part1	:HighLightMotionGlows.part
		part1	:HighLightMotionGlowsred.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet LLEGL

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:HighLightMotionGlowsBLK.part
		part1	:HighLightMotionGlows.part
		part1	:HighLightMotionGlowsred.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet FootL
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	:HighLightBodyGlowsBLK.part
		part1	:HighLightBodyGlows.part
		part2	:HighLightBodyGlowsRed.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:HighLightMotionGlowsBLK.part
		part1	:HighLightMotionGlows.part
		part1	:HighLightMotionGlowsred.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet LlegR

	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:HighLightMotionGlowsBLK.part
		part1	:HighLightMotionGlows.part
		part1	:HighLightMotionGlowsred.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet FootR
	End

	Event
		EName	9
		Type	Local
		At	Head
		part2	:HighLightHeadGlowsBLK.part
		part2	:HighLightHeadGlows.part
		part2	:HighLightHeadGlowsred.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:HighLightArmMotionGlowsBLK.part
		part1	:HighLightArmMotionGlows.part
		part1	:HighLightArmMotionGlowsred.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet LArmL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:HighLightArmMotionGlowsBLK.part
		part1	:HighLightArmMotionGlows.part
		part1	:HighLightArmMotionGlowsred.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet WepL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	:HighLightArmGlowsBLK.part
		part1	:HighLightArmGlows.part
		part1	:HighLightArmGlowsred.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:HighLightArmMotionGlowsBLK.part
		part1	:HighLightArmMotionGlows.part
		part1	:HighLightArmMotionGlowsred.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet LArmR

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:HighLightArmMotionGlowsBLK.part
		part1	:HighLightArmMotionGlows.part
		part1	:HighLightArmMotionGlowsred.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet WepR
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	:HighLightArmGlowsBLK.part
		part1	:HighLightArmGlows.part
		part1	:HighLightArmGlowsred.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	
	End

End

End