#############################################################
## Child_ElectricalBodySurge1.fx
#############################################################

FxInfo

Flags DontSuppress

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		part	:BodyGlowsLarge01.part
		part	:BodyArcsSmall01.part
		POther  UArmL
		Lifespan 6
	End
End

Condition
	On	Time
	Time	3

	Event
		ENAME	ChestGlow2
		Type	Local
		At	UArmL
		part	:BodyGlowsLarge01.part
		part	:BodyArcsSmall01.part
		POther  LArmL
		Lifespan 9
	End
End

Condition
	On	Time
	Time	6

	Event
		Type	Local
		At	LArmL
		part	:BodyGlowsLarge01.part
		part	:BodyArcsSmall01.part
		POther	WepL
		Lifespan 15
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		part	:BodyGlowsLarge01.part
		part	:BodyArcsSmall01.part
		POther  UArmR
		Lifespan 6
	End
End

Condition
	On	Time
	Time	3

	Event
		ENAME	ChestGlow2
		Type	Local
		At	UArmR
		part	:BodyGlowsLarge01.part
		part	:BodyArcsSmall01.part
		POther  LArmR
		Lifespan 9
	End
End

Condition
	On	Time
	Time	6

	Event
		Type	Local
		At	LArmR
		part	:BodyGlowsLarge01.part
		part	:BodyArcsSmall01.part
		POther	WepR
		Lifespan 15
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		part	:BodyGlowsLarge01.part
		part	:BodyArcsSmall01.part
		POther  Neck
		Lifespan 6
	End
End

Condition
	On	Time
	Time	3

	Event
		Type	Local
		At	Neck
		part	:BodyGlowsLarge01.part
		part	:BodyArcsSmall01.part
		POther  Hair
		Lifespan 9
	End
End

############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		part	:BodyGlowsLarge01.part
		part	:BodyArcsSmall01.part
		POther  ULegL
		Lifespan 8
	End
End

Condition
	On	Time
	Time	4

	Event
		ENAME	ChestGlow2
		Type	Local
		At	ULegL
		part	:BodyGlowsLarge01.part
		part	:BodyArcsSmall01.part
		POther  LLegL
		Lifespan 12
	End
End


Condition
	On	Time
	Time	8

	Event
		ENAME	ChestGlow2
		Type	Local
		At	LLegL
		part	:BodyGlowsLarge01.part
		part	:BodyArcsSmall01.part
		POther  FootL
		Lifespan 16
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		part	:BodyGlowsLarge01.part
		part	:BodyArcsSmall01.part
		POther  ULegR
		Lifespan 8
	End
End

Condition
	On	Time
	Time	4

	Event
		ENAME	ChestGlow2
		Type	Local
		At	ULegR
		part	:BodyGlowsLarge01.part
		part	:BodyArcsSmall01.part
		POther  LLegR
		Lifespan 12
	End
End

Condition
	On	Time
	Time	8

	Event
		Type	Local
		At	LLegR
		part	:BodyGlowsLarge01.part
		part	:BodyArcsSmall01.part
		POther	FootR
		Lifespan 16
	End
End

#############################################################

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset		3.0 0.0 0.5
			StartJitter		0.5 0.0 0.5
		End
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall02.part
		POther  WepR
		Lifespan 15
	End
End

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset		-3.0 0.0 0.5
			StartJitter		0.5 0.0 0.5
		End
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall02.part
		POther  WepL
		Lifespan 15
	End
End

#############################################################
Condition
	On	Time
	Time	0
	Random	1

	Event
		EName	Sound1
		Type	Local
		At	Origin
		Sound RobotSelfDestructElectricity_01 60 60 .8
		Lifespan 6
	End

	Event
		EName	Sound2
		Type	Local
		At	Origin
		Sound RobotSelfDestructElectricity_02 60 60 .8
	End
End
################################################################
End