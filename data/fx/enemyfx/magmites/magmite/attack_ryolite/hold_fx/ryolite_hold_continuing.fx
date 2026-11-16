#########################################################
##	Template

FxInfo

Condition
	On 	Time
	Time 	0
	
	Event
		EName	Chest
		Type	Local
		At	Root
		Part	:Lava_Bubbling_Rocks.part
		part	:Lava_Bubbling_01.part
		part	:Lava_Bubbling_02.part
		part	:Lava_Bubbling_04.part
		part	:Lava_Bubbling_Flat.part
		Sound volcano_loop 60 60 .55
		
	End

	Event
		EName	Chest
		Type	Local
		At	Chest
		part	:Spurts_01.part
		part	:Crest_01.part
		bhvr	:Lava_Top.bhvr
		Sound rumble2 60 60 .6

	End


	Event
		EName	Chest
		Type	Local
		At	LLegL
		part	:Spurts_02.part
		part	:Crest_01.part


	End

	Event
		EName	Chest
		Type	Local
		At	Root
		geom	FX_StoneArmor_LLEGR01
		bhvr	:Offset_Molten_Right.bhvr

	End

	Event
		EName	Chest
		Type	Local
		At	Root
		geom	FX_StoneArmor_LLEGL01
		bhvr	:Offset_Molten_Left.bhvr

	End

	Event
		EName	Chest
		Type	Local
		At	Root
		geom	FX_StoneArmorFootL01
		bhvr	:Offset_Molten_Front.bhvr

	End

	Event
		EName	Chest
		Type	Local
		At	Root
		geom	FX_StoneArmorFootL01
		bhvr	:Offset_Molten_Back.bhvr

	End

End

Condition
	On	Time
	Time	0
	

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	ENEMYFX\Magmites\Magmite\Head_Glow\InfernalBodyGlows01.part
		POther Neck	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Head
		part	ENEMYFX\Magmites\Magmite\Head_Glow\InfernalBodyStreaks01.part
		part	ENEMYFX\Magmites\Magmite\Head_Glow\InfernalBodyStreaks02.part
		part	ENEMYFX\Magmites\Magmite\Head_Glow\InfernalBodyGlows01.part
		POther Chest		
	End
	

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	ENEMYFX\Magmites\Magmite\Head_Glow\InfernalBodyGlows01.part
		POther LarmR	
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	ENEMYFX\Magmites\Magmite\Head_Glow\InfernalBodyStreaks01.part
		part	ENEMYFX\Magmites\Magmite\Head_Glow\InfernalBodyStreaks02.part
		part1	ENEMYFX\Magmites\Magmite\Head_Glow\InfernalBodyGlows01.part
		POther LarmR
		
	End


	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	ENEMYFX\Magmites\Magmite\Head_Glow\InfernalBodyGlows01.part
		POther LarmL
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	ENEMYFX\Magmites\Magmite\Head_Glow\InfernalBodyStreaks01.part
		part	ENEMYFX\Magmites\Magmite\Head_Glow\InfernalBodyStreaks02.part
		part1	ENEMYFX\Magmites\Magmite\Head_Glow\InfernalBodyGlows01.part
		POther LarmL
	End


	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part1	ENEMYFX\Magmites\Magmite\Head_Glow\InfernalBodyGlows01.part
		POther Ulegl		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part1	ENEMYFX\Magmites\Magmite\Head_Glow\InfernalBodyStreaks01.part
		part	ENEMYFX\Magmites\Magmite\Head_Glow\InfernalBodyStreaks02.part
		part1	ENEMYFX\Magmites\Magmite\Head_Glow\InfernalBodyGlows01.part
		POther Llegl
	End

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part1	ENEMYFX\Magmites\Magmite\Head_Glow\InfernalBodyGlows01.part
		POther UlegR	
	End

	Event	
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part1	ENEMYFX\Magmites\Magmite\Head_Glow\InfernalBodyStreaks01.part
		part	ENEMYFX\Magmites\Magmite\Head_Glow\InfernalBodyStreaks02.part
		part1	ENEMYFX\Magmites\Magmite\Head_Glow\InfernalBodyGlows01.part
		POther LlegR

	End	

End

End
