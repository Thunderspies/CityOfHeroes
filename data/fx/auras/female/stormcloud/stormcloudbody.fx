#########################################################
##	template

FxInfo


#########################################################
####################### AUDIO ###########################
#########################################################


Condition

	On 	Time
	Time	0

	Event
		Type Local
		At origin
		Sound eleccage_loop 70 70 .25
	End

End


#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	0


	Event
		Type	Local
		At	UarmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodyCloud.part
		Pmagnet UarmR
	End

	Event
		Type	Local
		At	FootR
		part	:BodyCloud.part
		Pmagnet FootR
	End

	Event
		Type	Local
		At	FootL
		part	:BodyCloud.part
		Pmagnet FootL
	End


	Event
		Type	Local
		At	UarmL
		part	:BodyCloud.part
		Pmagnet UarmL
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodyCloud.part
		part	:CloudTrail.part
		Pmagnet Neck
		POther  Neck

	End


	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodyCloud.part
		Pmagnet Hips
		POther  Hips
	End


	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:BodyCloud.part
		Pmagnet SpadL
		POther  SpadL
	End


	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodyCloud.part
		Pmagnet SpadR
		POther  SpadR
	End


	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodyCloud.part
		Pmagnet Chest
		POther  Chest
	End

	Event
		ENAME	LarmRNode
		Type	Local
		At	LarmR
		part	:CloudTrail.part
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		part	:BodyCloud.part
		Pmagnet LarmR
		POther  LarmR
	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:BodyCloud.part
		Pmagnet LarmR
		POther  LarmR
	End


	Event
		ENAME	LarmLNode
		Type	Local
		At	LarmL
		part	:CloudTrail.part
	End


	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:BodyCloud.part
		Pmagnet LarmL
		POther  LarmL
	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part	:BodyCloud.part
		Pmagnet Llegl
		POther  Llegl
	End


	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part	:BodyCloud.part
		Pmagnet LlegR
		POther  LlegR

	End


	Event
		Type	Local
		At	LlegR
		part	:BodyCloud.part
		part	:CloudTrail.part
		Pmagnet FootR
		POther  FootR
	End


	Event
		Type	Local
		At	LlegL
		part	:BodyCloud.part
		part	:CloudTrail.part
		Pmagnet FootL
		POther  FootL
	End


End


############################################################




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
		part	:BodyArcsSmall02.part
		Pmagnet SpadR
		POther  SpadR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 10

	End

End

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
		part	:BodyArcsSmall02.part
		Pmagnet SpadL
		POther  SpadL
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
		part	:BodyArcsSmall02.part
		Pmagnet Llegl
		POther  Llegl
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
		part	:BodyArcsSmall02.part
		Pmagnet LarmL
		POther  LarmL
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
		part	:BodyArcsSmall02.part
		Pmagnet LlegR
		POther  LlegR
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
		part	:BodyArcsSmall02.part
		Pmagnet LarmR
		POther  LarmR
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
		part	:BodyArcsSmall02.part
		Pmagnet LlegR
		POther  LlegR
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
		part	:BodyArcsSmall02.part
		Pmagnet Chest
		POther  Chest
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
		part	:BodyArcsSmall02.part
		Pmagnet LarmR
		POther  LarmR
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
		part	:BodyArcsSmall02.part
		Pmagnet Llegl
		POther  Llegl
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
		part	:BodyArcsSmall02.part
		Pmagnet LarmL
		POther  LarmL
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 10

	End

End