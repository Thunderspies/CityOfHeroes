#############################################################
## GlowBody.fx
#############################################################

FxInfo

Lifespan 30

## FORM SHIELD MATRIX ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\FlashGlow01.part
		POther  Neck
	End

	Event
		Type	Local
		At	Chest
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\FlashGlow01.part
		POther  Hips
	End

	Event
		Type	Local
		At	Hips
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\FlashGlow01.part
		POther  UArmL
	End

	Event
		Type	Local
		At	Hips
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\FlashGlow01.part
		POther  UarmR
	End
End

Condition
	On	Time
	Time	1

	Event
		Type	Local
		At	Neck
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\FlashGlow01.part
		POther  Hair
	End

	Event
		Type	Local
		At	UarmR
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\FlashGlow01.part
		POther  LarmR
	End

	Event
		Type	Local
		At	UarmL
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\FlashGlow01.part
		POther  LarmL
	End

	Event
		Type	Local
		At	UlegL
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\FlashGlow02.part
		POther  Llegl
	End

	Event
		Type	Local
		At	UlegR
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\FlashGlow02.part
		POther  LlegR
	End
End

Condition
	On	Time
	Time	2

	Event
		Type	Local
		At	LarmR
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\FlashGlow01.part
		POther  WepR
	End

	Event
		Type	Local
		At	LarmL
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\FlashGlow01.part
		POther  WepL
	End
End

Condition
	On	Time
	Time	3

	Event
		Type	Local
		At	LlegR
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\FlashGlow02.part
		POther  FootR
	End

	Event
		Type	Local
		At	LlegL
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\FlashGlow02.part
		POther  FootL
	End
End

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	FootR
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\FlashGlow01.part
		POther  ToeR
	End

	Event
		Type	Local
		At	FootL
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\FlashGlow01.part
		POther  ToeL
	End
End

#############################################################

End