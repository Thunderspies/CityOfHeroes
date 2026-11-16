FxInfo

Flags InheritAlpha


Condition
	On	Time
	Time	0

	Event	
		Type	local
		At	chest
		Sound Shivan2_loop 70 70 .64
	End
End



Condition
	On	Time
	Time	0
	

#########################################################
########################  HEAD ##########################
#########################################################

	Event	
		ENAME	HeadNode
		Type	Local
		At	Head		
	End

	Event	
		Type	Local
		At	Neck
		part	:ShivanAuraGlow_Col_R_01.part
		Pmagnet HeadNode
		POther  HeadNode	
	
	End

		Event	
		ENAME	Offset_Head_01
		Type	Local
		At	Chest
		Bhvr	:Offset_Head_01.bhvr
	End

	Event	 
		Type	Local
		At	Offset_Head_01
		part	:ShivanAuraGlow_Head_01.part	
	End

	Event	
		ENAME	EyeNodeL
		Type	Local
		At	Chest
		part	:EyeGlow01.part
		Bhvr	:EyeNodeL.bhvr
	End

	Event	
		ENAME	EyeNodeR
		Type	Local
		At	Chest
		part	:EyeGlow01.part
		Bhvr	:EyeNodeR.bhvr
	End

	Event	
		ENAME	EyeNodeL
		Type	Local
		At	Eyes
		#part	:EyeGlow02.part
	End

#########################################################
######################## CHEST ##########################
#########################################################

	Event	
		ENAME	NeckNode
		Type	Local
		At	Neck		
	End

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:ShivanAuraGlow_Col_R_01.part
		Pmagnet NeckNode
		POther  NeckNode	
	
	End

	Event	
		ENAME	HipsNode
		Type	Local
		At	Hips
		
	End

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:ShivanAuraGlow_Col_R_01.part
		Pmagnet HipsNode
		POther  HipsNode	
	
	End

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:ShivanAuraGlow_Col_R_01.part
		Pmagnet SpadLNode
		POther  SpadLNode	
	
	End

	Event	
		ENAME	SpadRNode
		Type	Local
		At	UarmR	
	End

	Event	
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		part	:ShivanAuraGlow_Col_R_01.part
		Pmagnet SpadRNode
		POther  SpadRNode	
	
	End

	Event	
		ENAME	ChestNode
		Type	Local
		At	Chest
	
	End

	Event	
		ENAME	SpadLNode
		Type	Local
		At	UArmL

	End

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:ShivanAuraGlow_Col_R_01.part
		Pmagnet SpadLNode
		POther  SpadLNode	
	
	End

	Event	
		ENAME	SpadRNode2
		Type	Local
		At	UarmR	
	End

	Event	
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		part	:ShivanAuraGlow_Col_R_01.part
		Pmagnet SpadRNode2
		POther  SpadRNode2	
	
	End

#########################################################
######################## ARMS ###########################
#########################################################

	Event	
		Type	Local
		At	UarmR
		part	:ShivanAuraGlow_Col_R_01.part
		Pmagnet UarmR
	End

	Event	
		Type	Local
		At	HandL
		part	:ShivanAuraGlow_Col_R_01.part
		Pmagnet HandL
	End

	Event	
		Type	Local
		At	HandR
		part	:ShivanAuraGlow_Col_R_01.part
		Pmagnet HandR
	End

	Event	
		Type	Local
		At	UarmL
		part	:ShivanAuraGlow_Col_R_01.part
		Pmagnet UarmL
	End

	Event	
		ENAME	LarmRNode
		Type	Local
		At	LarmR
	
	End

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		part	:ShivanAuraGlow_Col_R_01.part
		Pmagnet LarmRNode
		POther  LarmRNode
		
	End

	Event	
		ENAME	LarmRNode2
		Type	Local
		At	LarmR
	
	End

	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	HandR
		part	:ShivanAuraGlow_Col_R_01.part
		Pmagnet LarmRnode2
		POther  LarmRNode2
		
	End

	Event	
		ENAME	LarmLNode
		Type	Local
		At	LarmL
	
	End

	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		part	:ShivanAuraGlow_Col_R_01.part
		Pmagnet LarmLNode
		POther  LarmLNode
		
	End

	Event	
		ENAME	LarmLNode2
		Type	Local
		At	LarmL
	
	End

	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	HandL
		part	:ShivanAuraGlow_Col_R_01.part
		Pmagnet LarmLNode2
		POther  LarmLNode2
			
	End

	Event	
		ENAME	ShoulderNode
		Type	Local
		At	Neck
	
	End

	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	Col_L
		part	:ShivanAuraGlow_Col_R_01.part
		Pmagnet ShoulderNode
		POther  ShoulderNode
		
	End

	Event	
		ENAME	ShoulderNode2
		Type	Local
		At	Neck
	
	End

	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	Col_R
		part	:ShivanAuraGlow_Col_R_01.part
		Pmagnet ShoulderNode2
		POther  ShoulderNode2	
			
	End

#########################################################
######################## LEGS ###########################
#########################################################

	Event	
		Type	Local
		At	FootR
		part	:ShivanAuraGlow_Col_R_01.part
		Pmagnet FootR
	End

	Event	
		Type	Local
		At	FootL
		part	:ShivanAuraGlow_Col_R_01.part
		Pmagnet FootL
	End

	Event	
		ENAME	LleglNode
		Type	Local
		At	Llegl
	
	End

	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part	:ShivanAuraGlow_Col_R_01.part
		Pmagnet LleglNode
		POther  LleglNode
		
	End

	Event	
		ENAME	LlegRNode
		Type	Local
		At	LlegR
	End

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part	:ShivanAuraGlow_Col_R_01.part
		Pmagnet LlegRNode
		POther  LlegRNode
			
	End

	Event	 
		Type	Local
		At	LlegR
		part	:ShivanAuraGlow_Col_R_01.part
		Pmagnet FootR
		POther  FootR
	End

	Event	 
		Type	Local
		At	LlegL
		part	:ShivanAuraGlow_Col_R_01.part
		Pmagnet FootL
		POther  FootL
	End

#########################################################
######################## OTHER ##########################
#########################################################
	Event
		EName	Aura
		Type	Local
		At	Chest	
		part2	:Nucleus_Inner_03.part
		part2	:Nucleus_Inner_04.part
		part2	:Nucleus_Outer_02.part
		bhvr	:ShivanAuraGlow_Chest.bhvr
		
	End

End

End