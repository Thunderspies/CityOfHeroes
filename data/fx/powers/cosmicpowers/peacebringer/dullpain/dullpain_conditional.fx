#############################################################
## Peacebringer - Essence Boost
#############################################################

FxInfo
	
## BODY AURA ###########################################################

Condition
	On	Time
	Time	1

	Event
		Type	Local
		At	Head
		bhvr	Behaviors/GenericParticleFade.bhvr
		part2	:HeadGlows.part
		part	:RainbowGlows.part
		pMagnet	Head
	End

	Event
		Type	Local
		At	Col_L
		bhvr	Behaviors/GenericParticleFade.bhvr
		part2	:HeadGlows.part
		#part	:RainbowGlows.part
		pMagnet	Hips
	End

	Event
		Type	Local
		At	Col_R
		bhvr	Behaviors/GenericParticleFade.bhvr
		part2	:HeadGlows.part
		#part	:RainbowGlows.part
		pMagnet	Hips
	End

	Event
		Type	Local
		At	Hips
		bhvr	Behaviors/GenericParticleFade.bhvr
		part2	:HeadGlows.part
		#part	:RainbowGlows.part
		pMagnet	Hips
	End
	
##	Event
##		Type	Local
##		At	FootR
##		part1	:BodyGlows.part
##		part	:RainbowGlows.part
##		bhvr	Behaviors/GenericParticleFade.bhvr
##	End
##
##	Event
##		Type	Local
##		At	ULEGL
##		part1	:MotionGlows.part
##		part1	:BodyGlowsDown.part
##		part	:RainbowGlows.part
##		bhvr	Behaviors/GenericParticleFade.bhvr
##		PMagnet Knee1
##	End
##
##	Event
##		Type	Local
##		At	LLEGL
##		part1	:MotionGlows.part
##		part1	:BodyGlowsDown.part
##		part	:RainbowGlows.part
##		bhvr	Behaviors/GenericParticleFade.bhvr
##		PMagnet Chest
##	End
##
##	Event
##		Type	Local
##		At	FootL
##		part1	:BodyGlows.part
##		part	:RainbowGlows.part
##		bhvr	Behaviors/GenericParticleFade.bhvr
##	End
##
##	Event
##		Type	Local
##		At	ULEGR
##		part1	:MotionGlows.part
##		part1	:BodyGlowsDown.part
##		part	:RainbowGlows.part
##		bhvr	Behaviors/GenericParticleFade.bhvr
##		PMagnet	Kneer
##	End
##
##	Event
##		Type	Local
##		At	LLEGR
##		part1	:MotionGlows.part
##		part1	:BodyGlowsDown.part
##		part	:RainbowGlows.part
##		bhvr	Behaviors/GenericParticleFade.bhvr
##		PMagnet chest
##	End
##
##	Event
##		Type	Local
##		At	UArmL
##		part1	:ArmMotionGlows.part
##		part2	:ArmMotionGlows2.part
##		part	:RainbowGlows1.part
##		bhvr	Behaviors/GenericParticleFade.bhvr
##		PMagnet	ElbowL
##	End
##
##	Event
##		Type	Local
##		At	LArmL
##		part1	:ArmMotionGlows.part
##		part2	:ArmMotionGlows2.part
##		part	:RainbowGlows1.part
##		bhvr	Behaviors/GenericParticleFade.bhvr
##		PMagnet	HandL
##	End
##
##	Event
##		Type	Local
##		At	WepL
##		part1	:ArmGlows.part
##		part	:RainbowGlows1.part
##		bhvr	Behaviors/GenericParticleFade.bhvr
##	End
##
##	Event
##		Type	Local
##		At	UArmR
##		part1	:ArmMotionGlows.part
##		part2	:ArmMotionGlows2.part
##		part	:RainbowGlows1.part
##		bhvr	Behaviors/GenericParticleFade.bhvr
##		PMagnet	ElbowL
##	End
##
##	Event
##		Type	Local
##		At	LArmR
##		part1	:ArmMotionGlows.part
##		part2	:ArmMotionGlows2.part
##		part	:RainbowGlows1.part
##		bhvr	Behaviors/GenericParticleFade.bhvr
##		PMagnet	HandL
##	End
##
##	Event
##		Type	Local
##		At	WepR
##		part1	:ArmGlows.part
##		part	:RainbowGlows1.part
##		bhvr	Behaviors/GenericParticleFade.bhvr
##	End
End

#############################################################

Condition
	On 	cycle
	Time 	20
	Chance	0.125
	
	Event
		Type	Local
		At	Head
		part	:BigGrowingTendrils.part
		LifeSpan	25
	End
End

Condition
	On 	cycle
	Time 	19
	Chance	0.125
	
	Event
		Type	Local
		At	Col_L
		part	:BigGrowingTendrils.part
		LifeSpan	25
	End
End

Condition
	On 	cycle
	Time 	18
	Chance	0.125
	
	Event
		Type	Local
		At	Col_R
		part	:BigGrowingTendrils.part
		LifeSpan	25
	End
End

Condition
	On 	cycle
	Time 	17
	Chance	0.125
	
	Event
		Type	Local
		At	Hips
		part	:BigGrowingTendrils.part
		LifeSpan	25
	End
End

##Condition
##	On 	cycle
##	Time 	20
##	Chance	1 #0.2
##	
##	Event
##		Type	Local
##		At	LarmL
##		part	:RaysSmall.part
##		part	:BigGrowingTendrils.part
##		LifeSpan	25
##	End
##End
##
##Condition
##	On 	cycle
##	Time 	19
##	Chance	.2
##	
##	Event
##		Type	Local
##		At	LarmR
##		part	:RaysSmall.part
##		part	:BigGrowingTendrils.part
##		LifeSpan	25
##	End
##End
##
##Condition
##	On 	cycle
##	Time 	21
##	Chance	.2
##	
##	Event
##		Type	Local
##		At	UarmL
##		part	:RaysSmall.part
##		part	:BigGrowingTendrils.part
##		LifeSpan	25
##	End
##End
##
##Condition
##	On 	cycle
##	Time 	22
##	Chance	.2
##	
##	Event
##		Type	Local
##		At	UarmR
##		part	:RaysSmall.part
##		part	:BigGrowingTendrils.part
##		LifeSpan	25
##	End
##End
##
##Condition
##	On 	cycle
##	Time 	17
##	Chance	.2
##
##	Event
##		Type	Local
##		At	LlegL
##		part	:RaysSmall.part
##		part	:BigGrowingTendrils.part
##		LifeSpan	25
##	End
##End
##
##Condition
##	On 	cycle
##	Time 	18
##	Chance	.2
##
##	Event
##		Type	Local
##		At	LlegR
##		part	:RaysSmall.part
##		part	:BigGrowingTendrils.part
##		LifeSpan	25
##	End
##End

#############################################################

End