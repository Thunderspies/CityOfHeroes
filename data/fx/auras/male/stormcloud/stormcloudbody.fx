#############################################################
## StormCloudBody.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Hair
		part	:BodyCloud.part
		Pmagnet Chest
		POther  Chest
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:BodyCloud.part
		part	:CloudTrail.part
		Pmagnet Neck
		POther  Neck
	End

	Event
		Type	Local
		At	Chest
		part	:BodyCloud.part
		Pmagnet Hips
		POther  Hips
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:BodyCloud.part
		Pmagnet UArmL
		POther  UArmL
	End

	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		part	:BodyCloud.part
		Pmagnet UarmR
		POther  UarmR
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	UarmL
		part	:BodyCloud.part
		Pmagnet UarmL
	End

	Event
		Type	Local
		At	LarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:CloudTrail.part
		part	:BodyCloud.part
		Pmagnet UarmL
		POther  UarmL
	End

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyCloud.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		Type	Local
		At	UarmR
		part	:BodyCloud.part
		Pmagnet UarmR
	End

	Event
		Type	Local
		At	LarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:CloudTrail.part
		part	:BodyCloud.part
		Pmagnet UarmR
		POther  UarmR
	End

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyCloud.part
		Pmagnet LarmR
		POther  LarmR
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyCloud.part
		Pmagnet LlegL
		POther  LlegL
	End

	Event
		Type	Local
		At	LlegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyCloud.part
		part	:CloudTrail.part
		Pmagnet FootL
		POther  FootL
	End

	Event
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyCloud.part
		Pmagnet FootL
	End

	Event
		Type	Local
		At	UlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyCloud.part
		Pmagnet LlegR
		POther  LlegR
	End

	Event
		Type	Local
		At	LlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyCloud.part
		part	:CloudTrail.part
		Pmagnet FootR
		POther  FootR
	End

	Event
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyCloud.part
		Pmagnet FootR
	End
End

#############################################################

Condition
	On	Cycle
	Time	22
	Chance	0.05

	Event
		Type	Local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet SpadL
		POther  SpadL
		Lifespan 10
	End
End

Condition
	On	Cycle
	Time	20
	Chance	0.05

	Event
		Type	Local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet SpadR
		POther  SpadR
		Lifespan 10
	End
End

Condition
	On	Cycle
	Time	34
	Chance	0.05

	Event
		Type	Local
		At	Hair
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet Chest
		POther  Chest
		Lifespan 10
	End
End

Condition
	On	Cycle
	Time	31
	Chance	0.05

	Event
		Type	Local
		At	UarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet LarmR
		POther  LarmR
		Lifespan 10
	End
End

Condition
	On	Cycle
	Time	36
	Chance	0.05

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet LarmR
		POther  LarmR
		Lifespan 10
	End
End

Condition
	On	Cycle
	Time	40
	Chance	0.05

	Event
		Type	Local
		At	UarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet LarmL
		POther  LarmL
		Lifespan 10
	End
End

Condition
	On	Cycle
	Time	29
	Chance	0.05

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet LarmL
		POther  LarmL
		Lifespan 10
	End
End

Condition
	On	Cycle
	Time	37
	Chance	0.05

	Event
		Type	Local
		At	UlegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet Llegl
		POther  Llegl
		Lifespan 10
	End
End

Condition
	On	Cycle
	Time	28
	Chance	0.05

	Event
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet Llegl
		POther  Llegl
		Lifespan 10
	End
End

Condition
	On	Cycle
	Time	32
	Chance	0.05

	Event
		Type	Local
		At	UlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet LlegR
		POther  LlegR
		Lifespan 10
	End
End

Condition
	On	Cycle
	Time	30
	Chance	0.05

	Event
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet LlegR
		POther  LlegR
		Lifespan 10
	End
End

#############################################################

End