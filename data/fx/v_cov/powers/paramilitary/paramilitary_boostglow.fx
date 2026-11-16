#########################################################
##	chill touch hit
FxInfo

LifeSpan	150

##################################

Condition
	On	Time
	Time	0
	
	Event
		EName	Ring
		Type	Start
		At	Hips
		Sound heal1 70 70 .8
	End
End

#######################################################################################
#################################  start growing glow  ################################
#######################################################################################

Condition
	On	Time
	Time	0

	Event
		EName	1
		Type	Local
		At	UArmR
		part1	POWERS\Healing\HealingArmMotionGlows.part
		part2	POWERS\Healing\HealingArmMotionGlows2.part
		PMagnet ElbowL
		Lifespan 85

	End
End

#############################

Condition
	On	Time
	Time	3

	Event
		EName	2
		Type	Local
		At	LArmR
		part1	POWERS\Healing\HealingArmMotionGlows.part
		part2	POWERS\Healing\HealingArmMotionGlows2.part
		PMagnet HandL
		Lifespan 85
	End
End

#############################

Condition
	On	Time
	Time	6

	Event
		EName	3
		Type	Local
		At	Col_R
		part2	POWERS\Healing\HealingHeadGlows.part
		Lifespan 85
	End
End

#############################

Condition	
	On	Time
	Time	9

	Event
		EName	4
		Type	Local
		At	Hair
		part2	POWERS\Healing\HealingHeadGlows.part
		Lifespan 85
	End

	Event
		EName	4a
		Type	Local
		At	Neck
		part1	V_COV\Powers\Paramilitary\Paramilitary_P_BoostBigGlow.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 85
	End

	Event
		EName	5
		Type	Local
		At	WepR
		part1	POWERS\Healing\HealingArmGlows.part
		Lifespan 85	
	End
End

#############################

Condition	
	On	Time
	Time	12

	Event
		EName	6
		Type	Local
		At	UArmL
		part1	POWERS\Healing\HealingArmMotionGlows.part
		part2	POWERS\Healing\HealingArmMotionGlows2.part
		PMagnet ElbowL
		Lifespan 85
	End

	Event
		EName	7
		Type	Local
		At	Hips
		part1	V_COV\Powers\Paramilitary\Paramilitary_P_BoostBigGlow.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 85
	End
End

#############################

Condition	
	On	Time
	Time	15

	Event
		EName	8
		Type	Local
		At	LArmL
		part1	POWERS\Healing\HealingArmMotionGlows.part
		part2	POWERS\Healing\HealingArmMotionGlows2.part
		PMagnet HandL
		Lifespan 85
	End

	Event
		EName	9
		Type	Local
		At	ULEGR
		part1	POWERS\Healing\HealingMotionGlows.part
		PMagnet Kneer
		Lifespan 85
	End

	Event
		EName	10
		Type	Local
		At	ULEGL
		part1	POWERS\Healing\HealingMotionGlows.part
		PMagnet Knee1
		Lifespan 85

	End

End

#############################

Condition	
	On	Time
	Time	18

	Event
		EName	11
		Type	Local
		At	WepL
		part1	POWERS\Healing\HealingArmGlows.part
		Lifespan 85
	End

	Event
		EName	12
		Type	Local
		At	LLEGL
		part1	POWERS\Healing\HealingMotionGlows.part
		part1	V_COV\Powers\Paramilitary\Paramilitary_P_BoostBigGlow.part
		PMagnet FootL
		Lifespan 85
	End

	Event
		EName	13
		Type	Local
		At	LLEGR
		part1	POWERS\Healing\HealingMotionGlows.part
		part1	V_COV\Powers\Paramilitary\Paramilitary_P_BoostBigGlow.part
		PMagnet FootR
		Lifespan 85
	End
End

#############################

Condition
	On	Time
	Time	21
	
	Event
		EName	14
		Type	Local
		At	FootR
		part1	POWERS\Healing\HealingBodyGlows.part
		part2	POWERS\Healing\HealingBodyGlowsFlat.part
		Lifespan 85
	End

	Event
		EName	13		
		Type	Local
		At	FootL
		part1	POWERS\Healing\HealingBodyGlows.part
		part2	POWERS\Healing\HealingBodyGlowsFlat.part
		Lifespan 85
	End
End


#############################


End				