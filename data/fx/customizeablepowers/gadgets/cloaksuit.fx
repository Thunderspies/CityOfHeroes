#############################################################
## CloakSuit.fx
#############################################################

FxInfo
LifeSpan 30

#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On	Time
	Time	0


	Event
		Type	Local
		At 	Origin
		Sound rumble1 80 80 .8
	End
End


Condition
	On	Time
	Time	10


	Event
		Type	Local
		At 	Origin
		Sound invisible1 80 80 .5
	End
End
#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	10

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	CustomizeablePowers\Gadgets\CloakSuitGlow02.part
		part	CustomizeablePowers\Gadgets\CloakSuitGlowBlue.part
		POther LarmR
		Lifespan 0

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	CustomizeablePowers\Gadgets\CloakSuitGlow02.part
		part	CustomizeablePowers\Gadgets\CloakSuitGlowBlue.part
		POther LarmL
		Lifespan 0

	End

End

Condition
	On	Time
	Time	15

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	CustomizeablePowers\Gadgets\CloakSuitGlow02.part
		part	CustomizeablePowers\Gadgets\CloakSuitGlowBlue.part
		POther LarmR
		Lifespan 0

	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	CustomizeablePowers\Gadgets\CloakSuitGlow02.part
		part	CustomizeablePowers\Gadgets\CloakSuitGlowBlue.part
		POther LarmL
		Lifespan 0

	End


	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	CustomizeablePowers\Gadgets\CloakSuitGlow02.part
		part	CustomizeablePowers\Gadgets\CloakSuitGlowBlue.part
		POther Neck
		Lifespan 0

	End

End

Condition
	On	Time
	Time	18

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	CustomizeablePowers\Gadgets\CloakSuitGlow02.part
		part	CustomizeablePowers\Gadgets\CloakSuitGlowBlue.part
		POther Chest
		Lifespan 0

	End

End

Condition
	On	Time
	Time	5

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part	CustomizeablePowers\Gadgets\CloakSuitGlow02.part
		part	CustomizeablePowers\Gadgets\CloakSuitGlowBlue.part
		POther Ulegl
		Lifespan 0

	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part	CustomizeablePowers\Gadgets\CloakSuitGlow02.part
		part	CustomizeablePowers\Gadgets\CloakSuitGlowBlue.part
		POther UlegR
		Lifespan 0

	End

End

Condition
	On	Time
	Time	0

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	CustomizeablePowers\Gadgets\CloakSuitGlow01.part
		part	CustomizeablePowers\Gadgets\CloakSuitGlowBlue.part
		POther Llegl
		Lifespan 23

	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	CustomizeablePowers\Gadgets\CloakSuitGlow01.part
		part	CustomizeablePowers\Gadgets\CloakSuitGlowBlue.part
		POther LlegR
		Lifespan 23

	End


End


End