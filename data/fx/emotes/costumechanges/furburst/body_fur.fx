#############################################################
## Body_Fur.fx
#############################################################

FxInfo




#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	0


	Event
		Type	Local
		At	Head
		part	:Fur_Burst.part
		Part	:Fur_Burst_Core.part
		Part	:Fur_Loose.part
	End

	Event
		Type	Local
		At	UarmL
		part	:Fur_Burst.part
		Part	:Fur_Loose.part

	End

	Event
		Type	Local
		At	UarmR
		part	:Fur_Burst.part
		Part	:Fur_Loose_Lingering.part
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		part	:Fur_Burst.part
		Part	:Fur_Loose.part
		POther  LarmR
	End


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmL
		part	:Fur_Burst.part
		Part	:Fur_Loose.part
		POther  LarmL
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepL
		part	:Fur_Burst.part
		Pmagnet LArmL
		POther  LArmL
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:Fur_Burst.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:Fur_Burst.part
		Part	:Fur_Loose_Lingering.part
		POther  Neck
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:Fur_Burst.part
		Part	:Fur_Burst_Core.part
		POther  Hips
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	ULegL
		part	:Fur_Burst.part
		Part	:Fur_Burst_Core.part
		POther  LLegL
	End


	Event
		ENAME	RightULegGlow
		Type	Local
		At	ULegR
		part	:Fur_Burst.part
		Part	:Fur_Burst_Core.part
		POther  LLegR
	End

	Event
		Type	Local
		At	LlegR
		part	:Fur_Burst.part
		Part	:Fur_Burst_Core.part
		Part	:Fur_Loose.part
		POther  ToeR
	End

	Event
		Type	Local
		At	LlegL
		part	:Fur_Burst.part
		Part	:Fur_Burst_Core.part
		Part	:Fur_Loose_Lingering.part
		POther  ToeL
	End

	Event
		Type	Local
		At	ToeL
		part	:Fur_Burst.part
		Part	:Fur_Burst_Core.part
	End

	Event
		Type	Local
		At	ToeR
		part	:Fur_Burst.part
		Part	:Fur_Burst_Core.part
	End
End



End