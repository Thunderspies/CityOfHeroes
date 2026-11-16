#########################################################
##	template

FxInfo

Flags InheritAlpha

Condition
	On 	Time
	Time 	0	

####################  CHEST  ###########################

	Event	
		ENAME	HipsNode
		Type	Local
		At	Hips	
	End

	Event	
		ENAME	ChestGlow1
		Type	Local
		At	Col_R
		part	:Lava_Chest_01.part
		part	:Inner_Glow_Col_01.part
		Bhvr	:Col_R_Offset.bhvr
		Pmagnet HipsNode
		POther  HipsNode
	End

	Event	
		ENAME	ChestGlow2
		Type	Local
		At	Col_L
		part	:Lava_Chest_01.part
		part	:Inner_Glow_Col_01.part
		Bhvr	:Col_L_Offset.bhvr
		Pmagnet HipsNode
		POther  HipsNode
	End

	Event	
		ENAME	Chest
		Type	Local
		At	Chest
		part	:Lava_ChestR.part
		Part	:Inner_Glow_Chest_02.part
		Bhvr	:ChestR_Offset.bhvr
	End

	Event	
		ENAME	Chest
		Type	Local
		At	Chest
		part	:Lava_ChestR.part
		Part	:Inner_Glow_Chest_02.part
		Bhvr	:ChestR_Back_Offset.bhvr
	End

	Event	
		ENAME	Chest
		Type	Local
		At	Chest
		part	:Lava_ChestR.part
		Part	:Inner_Glow_Chest_02.part
		Bhvr	:ChestL_Offset.bhvr
	End

	Event	
		ENAME	Chest
		Type	Local
		At	Chest
		part	:Lava_ChestR.part
		Part	:Inner_Glow_Chest_02.part
		Bhvr	:ChestL_Back_Offset.bhvr
	End

	Event	
		ENAME	Waist
		Type	Local
		At	Waist
		part	:Lava_Waist_01.part
		Part	:Inner_Glow_Waist_01.part
		Part	:Inner_Glow_Waist_02.part
	End

	Event	
		ENAME	Neckline
		Type	Local
		At	Neckline
		part	:Lava_Chest_01.part
		Part	:Inner_Glow_Col_01.part
		Bhvr	:Neckline_Offset.bhvr
	End

	Event	
		ENAME	ChestPulse
		Type	Local
		At	Chest
		part	:Inner_Glow_Heart.part
		Bhvr	:Chest_Heart_Offset.bhvr
	End

	Event	
		ENAME	ChestPulse2
		Type	Local
		At	Chest
		part	:Inner_Glow_Heart.part
		Bhvr	:Chest_Heart_Offset_02.bhvr
	End

	Event	
		ENAME	ChestPulse3
		Type	Local
		At	Chest
		part	:Inner_Glow_Heart.part
		Bhvr	:Chest_Heart_Offset_03.bhvr
	End

	Event	
		ENAME	ChestPulse3
		Type	Local
		At	Chest
		part	:Inner_Glow_Heart.part
		Bhvr	:Chest_Heart_Offset_04.bhvr
	End

####################  ULegR  ###########################

	Event	
		ENAME	LLegR_Node
		Type	Local
		At	LLegR	
	End

	Event	
		ENAME	ULegR
		Type	Local
		At	ULegR
		part	:Lava_ULegR_01.part
		part	:Inner_Glow_ULegR_01.part
		Pmagnet LLegR_Node
		POther  LLegR_Node
	End

####################  LLegR  ###########################

	Event	
		ENAME	FootR_Node
		Type	Local
		At	FootR	
	End

	Event	
		ENAME	LLegR
		Type	Local
		At	LLegR
		part	:Lava_LLegR_01.part
		part	:Inner_Glow_ULegR_01.part
		Pmagnet FootR_Node
		POther  FootR_Node
	End

	Event	
		ENAME	FootBase
		Type	Local
		At	FootR
		#part	:Inner_Glow_FootBase.part
		Bhvr	:FootBase_Offset.bhvr

	End


####################  ULegL  ###########################

	Event	
		ENAME	LLegL_Node
		Type	Local
		At	LLegL	
	End

	Event	
		ENAME	ULegL
		Type	Local
		At	ULegL
		part	:Lava_ULegR_01.part
		part	:Inner_Glow_ULegL_01.part
		Pmagnet LLegL_Node
		POther  LLegL_Node
	End

####################  LLegL  ###########################

	Event	
		ENAME	FootL_Node
		Type	Local
		At	FootL	
	End

	Event	
		ENAME	LLegL
		Type	Local
		At	LLegL
		part	:Lava_LLegR_01.part
		part	:Inner_Glow_ULegR_01.part
		Pmagnet FootL_Node
		POther  FootL_Node
	End

	Event	
		ENAME	FootBase
		Type	Local
		At	FootL
		#part	:Inner_Glow_FootBase.part
		Bhvr	:FootBase_Offset.bhvr

	End

####################  FootR  ###########################

	Event	
		ENAME	FootR
		Type	Local
		At	FootR
		Part	:Inner_Glow_FootR.part
		part	:Lava_Waist_01.part

	End

####################  FootL  ###########################

	Event	
		ENAME	FootL
		Type	Local
		At	FootL
		Part	:Inner_Glow_FootR.part
		part	:Lava_Waist_01.part

	End

End
