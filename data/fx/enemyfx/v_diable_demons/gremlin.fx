FxInfo

Flags InheritAlpha


Condition
	On	Time
	Time	0

	Event	
		Type	local
		At	chest
		Sound eleccage_loop 20 20 0.4
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
		At	Neck		
	End

	Event	
		Type	Local
		At	Neck
		#part	:DemonAuraGlow_Generic.part
		Pmagnet HeadNode
		POther  HeadNode
	
	End

		Event	
		ENAME	Offset_Head_01
		Type	Local
		At	Neck
	End

	Event	 
		Type	Local
		At	Offset_Head_01
		part	:DemonAuraGlow_Head_01.part	
	End

		Event	 
		Type	Local
		At	Head
		part	:DemonHead.part	
	End

	Event	
		ENAME	Offset_Head_02
		Type	Local
		At	Head
		Bhvr	:Offset_Head_01.bhvr
	End
	Event	 
		Type	Local
		At	Head
		part	:DemonAuraGlow_Head_02.part	
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
		At	Neck
		part	:DemonAuraGlow_Chest.part
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
		part	:DemonAuraGlow_Chest.part
		Pmagnet HipsNode
		POther  HipsNode	
	
	End

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:DemonAuraGlow_Chest.part
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
		part	:DemonAuraGlow_Chest.part
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
		part	:DemonAuraGlow_Chest.part
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
		part	:DemonAuraGlow_Chest.part
		Pmagnet SpadRNode2
		POther  SpadRNode2	
	
	End

	Event	
		ENAME	ChestInnerGlow
		Type	Local
		At	Chest
		#part	:DemonAuraGlow_Chest_Innerglow.part	
	
	End
	Event	
		ENAME	ChestInnerGlow
		Type	Local
		At	Hips
		part	:DemonAuraGlow_Chest_Innerglow_02.part	
	
	End

#########################################################
######################## ARMS ###########################
#########################################################

	Event	
		Type	Local
		At	UarmR
		part	:DemonAuraGlow_Arms.part
		Pmagnet UarmR
	End

	Event	
		Type	Local
		At	HandL
		part	:DemonAuraGlow_Hands.part
		Pmagnet HandL
	End

	Event	
		Type	Local
		At	HandL
		part	:DemonAuraGlow_HandsFist.part
		Pmagnet HandL
	End

	Event	
		Type	Local
		At	HandR
		part	:DemonAuraGlow_Hands.part
		Pmagnet HandR
	End

	Event	
		Type	Local
		At	HandR
		part	:DemonAuraGlow_HandsFist.part
		Pmagnet HandR
	End

	Event	
		Type	Local
		At	UarmL
		part	:DemonAuraGlow_Arms.part
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
		part	:DemonAuraGlow_Arms.part
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
		part	:DemonAuraGlow_Arms.part
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
		part	:DemonAuraGlow_Arms.part
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
		part	:DemonAuraGlow_Arms.part
		Pmagnet LarmLNode2
		POther  LarmLNode2
			
	End

	Event	
		ENAME	ShoulderNode
		Type	Local
		At	back
	
	End

	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	UArmL
		part	:DemonAuraGlow_Hands.part
	End

	Event	
		ENAME	ShoulderNode2
		Type	Local
		At	back
	
	End

	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	UArmR
		part	:DemonAuraGlow_Hands.part		
	End

#########################################################
######################## LEGS ###########################
#########################################################

	Event	
		Type	Local
		At	FootR
		part	:DemonAuraGlow_Legs.part
		Pmagnet FootR
	End

	Event	
		Type	Local
		At	FootL
		part	:DemonAuraGlow_Legs.part
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
		part	:DemonAuraGlow_Legs.part
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
		part	:DemonAuraGlow_Legs.part
		Pmagnet LlegRNode
		POther  LlegRNode
			
	End

	Event	 
		Type	Local
		At	LlegR
		part	:DemonAuraGlow_Legs.part
		Pmagnet FootR
		POther  FootR
	End

	Event	 
		Type	Local
		At	LlegL
		part	:DemonAuraGlow_Legs.part
		Pmagnet FootL
		POther  FootL
	End

	Event	 
		Type	Local
		At	FootL
		part	:DemonAuraGlow_Generic.part
	End

	Event	 
		Type	Local
		At	FootL
		#part	:DemonAuraGlow_Feet.part
	End

	Event	 
		Type	Local
		At	FootR
		#part	:DemonAuraGlow_Feet.part
	End

	Event	 
		Type	Local
		At	FootR
		part	:DemonAuraGlow_Col_R_01.part
	End
	Event	
		ENAME	ChestInnerGlow
		Type	Local
		At	UlegR
		part	:DemonAuraGlow_Chest.part	
	
	End

	Event	
		ENAME	ChestInnerGlow
		Type	Local
		At	UlegL
		part	:DemonAuraGlow_Chest.part	
	
	End


End

End
