#########################################################
##	chill touch hit
FxInfo

########################################################################
##Light Straks
#####################################

######################################################################

Condition
	On 	cycle
	Time 	20
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	Head
		
		part	:BigGrowingTendrils.part

		LifeSpan	25
	End
	
End

Condition
	On 	cycle
	Time 	20
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	LarmL
		part	:RaysSmall.part
		part	:Ring2.part
		part	:Ring.part
		part	:BigGrowingTendrils.part

		LifeSpan	25
	End
	
End

Condition
	On 	cycle
	Time 	19
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	LarmR
		part	:RaysSmall.part
		part	:Ring2.part
		part	:Ring.part
		part	:BigGrowingTendrils.part

		LifeSpan	25
	End
	
End

Condition
	On 	cycle
	Time 	21
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	UarmL
		part	:RaysSmall.part
		part	:Ring2.part
		part	:Ring.part
		part	:BigGrowingTendrils.part
		
		LifeSpan	25
	End
	
End


Condition
	On 	cycle
	Time 	22
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	UarmR
		part	:RaysSmall.part
		part	:Ring2.part
		part	:Ring.part
		part	:BigGrowingTendrils.part
		
		LifeSpan	25
	End
	

End

Condition
	On 	cycle
	Time 	17
	Chance	.2

	Event
		EName	3
		Type	Local
		At	LlegL
		part	:RaysSmall.part
		part	:Ring2.part
		part	:Ring.part
		part	:BigGrowingTendrils.part
		
		LifeSpan	25
	End

End

Condition
	On 	cycle
	Time 	18
	Chance	.2

	Event
		EName	3
		Type	Local
		At	LlegR
		part	:RaysSmall.part
		part	:Ring2.part
		part	:Ring.part
		part	:BigGrowingTendrils.part
		
		LifeSpan	25
	End


End

########################################################################
##Aura Parts
#####################################

Condition
	On	Time
	Time	18
	
	Event
		EName	2
		Type	Local
		At	FootR
		part1	:BodyGlows.part
		part	:RainbowGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		
	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:MotionGlows.part
		part1	:BodyGlowsDown.part
		part	:RainbowGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet Knee1

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:MotionGlows.part
		part1	:BodyGlowsDown.part
		part	:RainbowGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet Chest
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	:BodyGlows.part
		part	:RainbowGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:MotionGlows.part
		part1	:BodyGlowsDown.part
		part	:RainbowGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet Kneer

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:MotionGlows.part
		part1	:BodyGlowsDown.part
		part	:RainbowGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet chest
	End

###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		part2	:HeadGlows.part
		part	:RainbowGlows1.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:ArmMotionGlows.part
		part2	:ArmMotionGlows2.part
		part	:RainbowGlows1.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet ElbowL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:ArmMotionGlows.part
		part2	:ArmMotionGlows2.part
		part	:RainbowGlows1.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	:ArmGlows.part
		part	:RainbowGlows1.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	
	End






	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:ArmMotionGlows.part
		part2	:ArmMotionGlows2.part
		part	:RainbowGlows1.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet ElbowL

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:ArmMotionGlows.part
		part2	:ArmMotionGlows2.part
		part	:RainbowGlows1.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	:ArmGlows.part
		part	:RainbowGlows1.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	
	End



End


End				