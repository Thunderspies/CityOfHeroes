#########################################################
##	template

FxInfo

Lifespan 300

#########################################################
####################### AUDIO ###########################
#########################################################



#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Cycle
	Time	22
	Chance	.30



	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmallSub.part
		Pmagnet SpadL
		POther  SpadL
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 10

	End

End


Condition
	On	Cycle
	Time	20
	Chance	.30



	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmallSub.part
		Pmagnet SpadR
		POther  SpadR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 10

	End

End

Condition
	On	Cycle
	Time	34
	Chance	.25



	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmallSub.part
		Pmagnet Chest
		POther  Chest
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 10

	End

End

Condition
	On	Cycle
	Time	31
	Chance	.25


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmallSub.part
		Pmagnet LarmR
		POther  LarmR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 10

	End

End

Condition
	On	Cycle
	Time	36
	Chance	.25


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmallSub.part
		Pmagnet LarmR
		POther  LarmR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 10

	End

End

Condition
	On	Cycle
	Time	40
	Chance	.25



	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmallSub.part
		Pmagnet LarmL
		POther  LarmL
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 10

	End

End

Condition
	On	Cycle
	Time	29
	Chance	.25



	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmallSub.part
		Pmagnet LarmL
		POther  LarmL
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 10

	End
End

Condition
	On	Cycle
	Time	37
	Chance	.25



	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmallSub.part
		Pmagnet Llegl
		POther  Llegl
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 10

	End
End

Condition
	On	Cycle
	Time	28
	Chance	.25



	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmallSub.part
		Pmagnet Llegl
		POther  Llegl
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 10

	End
End

Condition
	On	Cycle
	Time	32
	Chance	.25


	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmallSub.part
		Pmagnet LlegR
		POther  LlegR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 10

	End
End

Condition
	On	Cycle
	Time	30
	Chance	.25



	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmallSub.part
		Pmagnet LlegR
		POther  LlegR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 10

	End


End


End