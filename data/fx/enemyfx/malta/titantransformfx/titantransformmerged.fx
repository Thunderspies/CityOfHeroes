#############################################################
## TitanTransformMerged.fx
#############################################################

FxInfo

Lifespan 50

#############################################################

Condition
	On 	Time
	Time	0

	Event
		Type	Local
		At 	Chest
		Bhvr	Behaviors\SSThunderClapShake.bhvr
		Sound SSthunder 120 120 1.0
	End

	Event
		ENAME	Flash
		Type	Local
		At	Chest
		part	:TitanTransformFlash.part
		part	:TitanTransformLightning.part
		part	:TitanTransformFlashGlow.part
		LifeSpan 12
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	BackGlow1
		Type	Local
		At	Back
		Bhvr	:TitanTransformBackOffset1.bhvr
		part	:ElectricArcsLarge01.part
		part	:ElectricArcsLarge02.part
		Pmagnet Neck
		POther  Neck
		Lifespan 0
	End

	Event
		ENAME	BackGlow2
		Type	Local
		At	Back
		Bhvr	:TitanTransformBackOffset2.bhvr
		part	:ElectricArcsLarge01.part
		part	:ElectricArcsLarge02.part
		Pmagnet Neck
		POther  Neck
		Lifespan 0
	End

	Event

		ENAME	BackGlow3
		Type	Local
		At	Back
		Bhvr	:TitanTransformBackOffset2b.bhvr
		part	:ElectricArcsLarge01.part
		part	:ElectricArcsLarge02.part
		Pmagnet Neck
		POther  Neck
		Lifespan 0
	End

	Event

		ENAME	BackGlow4
		Type	Local
		At	Back
		Bhvr	:TitanTransformBackOffset1b.bhvr
		part	:ElectricArcsLarge01.part
		part	:ElectricArcsLarge02.part
		Pmagnet Neck
		POther  Neck
		Lifespan 0
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:ElectricArcsLarge01.part
		part	:ElectricArcsLarge02.part
		Pmagnet Neck
		POther  Neck
		Lifespan 0
	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	:ElectricArcsLarge01.part
		part	:ElectricArcsSmall01.part
		Pmagnet Chest
		POther  Chest
		Lifespan 0
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:ElectricArcsLarge01.part
		part	:ElectricArcsSmall01.part
		Pmagnet LarmR
		POther  LarmR
		Lifespan 0
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:ElectricArcsLarge01.part
		part	:ElectricArcsSmall01.part
		Pmagnet LarmR
		POther  LarmR
		Lifespan 0
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:ElectricArcsLarge01.part
		part	:ElectricArcsSmall01.part
		Pmagnet LarmL
		POther  LarmL
		Lifespan 0
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:ElectricArcsLarge01.part
		part	:ElectricArcsSmall01.part
		Pmagnet LarmL
		POther  LarmL
		Lifespan 0
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part	:ElectricArcsLarge01.part
		part	:ElectricArcsSmall01.part
		Pmagnet LlegL
		POther  LlegL
		Lifespan 1
	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	:ElectricArcsLarge01.part
		part	:ElectricArcsSmall01.part
		Pmagnet LlegL
		POther  LlegL
		Lifespan 1
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part	:ElectricArcsLarge01.part
		part	:ElectricArcsSmall01.part
		Pmagnet LlegR
		POther  LlegR
		Lifespan 1

	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:ElectricArcsLarge01.part
		part	:ElectricArcsSmall01.part
		Pmagnet LlegR
		POther  LlegR
		Lifespan 1
	End
End

#############################################################

End

