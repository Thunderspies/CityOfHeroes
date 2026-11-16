#########################################################
##	chill touch hit
FxInfo

###################################################################

Condition
	On	Time
	Time	80

	Event
		Type	Local
		At	hips
		Sound libertybelt_loop 70 70 .6
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 620
	End
End


Condition
#	On	Time
#	Time	40

	Event
		EName	1
		Type	Local
		At	Hips
#		part1	:Ms_Liberty_Highlight01.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		#Sound	forcefield5_loop 70 60 .6
	End

	Event
		EName	2
		Type	Local
		At	FootR
		part1	:Ms_Liberty_FeetGlows.part
		part1	:Ms_Liberty_BodyGlowsBlue.part
		part1	:Ms_Liberty_BodyGlowsRed.part
		Part	:Ms_Liberty_Sparklies.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:Ms_Liberty_MotionGlows.part
		part1	:Ms_Liberty_MotionGlowsBlue.part
		part1	:Ms_Liberty_MotionGlowsRed.part
		Part	:Ms_Liberty_Sparklies.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet LLEGL

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:Ms_Liberty_MotionGlows.part
		part1	:Ms_Liberty_MotionGlowsBlue.part
		part1	:Ms_Liberty_MotionGlowsRed.part
		Part	:Ms_Liberty_Sparklies.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet FootL
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	:Ms_Liberty_FeetGlows.part
		part1	:Ms_Liberty_BodyGlowsBlue.part
		part1	:Ms_Liberty_BodyGlowsRed.part
		Part	:Ms_Liberty_Sparklies.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:Ms_Liberty_MotionGlows.part
		part1	:Ms_Liberty_MotionGlowsBlue.part
		part1	:Ms_Liberty_MotionGlowsRed.part
		Part	:Ms_Liberty_Sparklies.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet LlegR

	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:Ms_Liberty_MotionGlows.part
		part1	:Ms_Liberty_MotionGlowsBlue.part
		part1	:Ms_Liberty_MotionGlowsRed.part
		Part	:Ms_Liberty_Sparklies.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet FootR
	End

	Event
		EName	9
		Type	Local
		At	Head
		part2	:Ms_Liberty_HeadGlows.part
		part2	:Ms_Liberty_HeadGlowsBlue.part
		part2	:Ms_Liberty_HeadGlowsRed.part
		Part	:Ms_Liberty_Sparklies.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:Ms_Liberty_ArmMotionGlows.part
		part1	:Ms_Liberty_ArmMotionGlowsBlue.part
		part1	:Ms_Liberty_ArmMotionGlowsRed.part
		Part	:Ms_Liberty_Sparklies.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet LArmL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:Ms_Liberty_ArmMotionGlows.part
		part1	:Ms_Liberty_ArmMotionGlowsBlue.part
		part1	:Ms_Liberty_ArmMotionGlowsRed.part
		Part	:Ms_Liberty_Sparklies.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet WepL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	:Ms_Liberty_ArmGlows.part
		part1	:Ms_Liberty_ArmGlowsBlue.part
		part1	:Ms_Liberty_ArmGlowsRed.part
		Part	:Ms_Liberty_Sparklies.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:Ms_Liberty_ArmMotionGlows.part
		part1	:Ms_Liberty_ArmMotionGlowsBlue.part
		part1	:Ms_Liberty_ArmMotionGlowsRed.part
		Part	:Ms_Liberty_Sparklies.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet LArmR

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:Ms_Liberty_ArmMotionGlows.part
		part1	:Ms_Liberty_ArmMotionGlowsBlue.part
		part1	:Ms_Liberty_ArmMotionGlowsRed.part
		Part	:Ms_Liberty_Sparklies.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet WepR
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	:Ms_Liberty_ArmGlows.part
		part1	:Ms_Liberty_ArmGlowsBlue.part
		part1	:Ms_Liberty_ArmGlowsRed.part
		Part	:Ms_Liberty_Sparklies.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	
	End

End

End