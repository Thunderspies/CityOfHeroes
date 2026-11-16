#############################################################
## CloakSuitContinuing.fx
#############################################################


FxInfo

#LifeSpan 30


#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On	Time
	Time	10


	Event
		Type	Local
		At 	Origin
		Sound glow8_loop 40 40 .5
		bhvr	behaviors/Sound/SoundIn2Out10.bhvr
		lifespan 4
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
		part	:CloakSuitGlow02.part
		POther LarmR
		Lifespan 0

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:CloakSuitGlow02.part
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
		part	:CloakSuitGlow02.part
		POther LarmR
		Lifespan 0

	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:CloakSuitGlow02.part
		POther LarmL
		Lifespan 0

	End


	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:CloakSuitGlow02.part
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
		part	:CloakSuitGlow02.part
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
		part	:CloakSuitGlow02.part
		POther Ulegl
		Lifespan 0

	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part	:CloakSuitGlow02.part
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
		part	:CloakSuitGlow01a.part
		POther Llegl
	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:CloakSuitGlow01a.part
		POther LlegR
	End


End

#########################################################

End