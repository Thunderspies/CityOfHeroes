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
		EName	1
		Type	Local
		At	Hips
		part1	Powers/LightControl/HighLight01.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		#Sound	forcefield5_loop 70 60 .6
	End


	Event
		EName	9
		Type	Local
		At	Hair
		part2	Powers/LightControl/HighLightHeadGlows.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	Powers/LightControl/HighLightArmMotionGlows.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet LArmL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	Powers/LightControl/HighLightArmMotionGlows.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet WepL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	Powers/LightControl/HighLightArmGlows.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	Powers/LightControl/HighLightArmMotionGlows.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet LArmR

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	Powers/LightControl/HighLightArmMotionGlows.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet WepR
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	Powers/LightControl/HighLightArmGlows.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	
	End

End

End