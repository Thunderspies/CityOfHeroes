#########################################################
##	template

FxInfo


Flags InheritAnimScale InheritAlpha

#########################################################
####################### AUDIO ###########################
#########################################################




#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	0
	
	Event	
		ENAME	HeadNode
		Type	Local
		At	Head
		Bhvr	:HeadOffset1.bhvr
	End

	Event	
		Type	Local
		At	HeadNode
		part	:BodyAura02.part
		Pmagnet Head
	End

		Event	
		Type	Local
		At	Head
		part	:BodyAura02.part
		part	:BodyAura02.part
		part	:BodyAura_Mist.part
		Bhvr	:HeadOffset1.bhvr
	End

	Event	
		Type	Local
		At	UarmR
		part	:BodyAura02.part
		Pmagnet UarmR
	End

	Event	
		Type	Local
		At	FootR
		part	:BodyAura02.part
		Pmagnet FootR
	End

	Event	
		Type	Local
		At	FootL
		part	:BodyAura02.part
		Pmagnet FootL
	End


	Event	
		Type	Local
		At	UarmL
		part	:BodyAura02.part
		Pmagnet UarmL
	End


	Event	
		ENAME	NeckNode
		Type	Local
		At	Neck		
	End

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:BodyAura02.part
		Pmagnet NeckNode
		POther  NeckNode	
	
	End

End

Condition
	On	Time
	Time	0


	Event	
		ENAME	HipsNode
		Type	Local
		At	Hips
		
	End

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:BodyAura02.part
		Pmagnet HipsNode
		POther  HipsNode	
	
	End

	Event	
		ENAME	HipsMist
		Type	Local
		At	Hips
		part	:BodyAura_Mist.part
	End


End

Condition
	On	Time
	Time	0


	Event	
		ENAME	SpadLNode
		Type	Local
		At	UArmL


	End

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:BodyAura02.part
		Pmagnet SpadLNode
		POther  SpadLNode	
	
	End

End


Condition
	On	Time
	Time	0


	Event	
		ENAME	SpadRNode
		Type	Local
		At	UarmR	
	End

	Event	
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		part	:BodyAura02.part
		Pmagnet SpadRNode
		POther  SpadRNode
		
		
	
	End

End

Condition
	On	Time
	Time	0
	

	Event	
		ENAME	ChestNode
		Type	Local
		At	Chest
	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	:BodyAura02.part
		Pmagnet ChestNode
		POther  ChestNode
		
		
		
	End

End

Condition
	On	Time
	Time	0
	
	
	Event	
		ENAME	LarmRNode
		Type	Local
		At	LarmR
	
	End

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		part	:BodyAura02.part
		Pmagnet LarmRNode
		POther  LarmRNode
		
		
		
	End

End

Condition
	On	Time
	Time	0
	
	
	Event	
		ENAME	LarmRNode2
		Type	Local
		At	LarmR
	
	End

	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:BodyAura02.part
		Pmagnet LarmRnode2
		POther  LarmRNode2
		
		
		
	End

End

Condition
	On	Time
	Time	0
	

	Event	
		ENAME	LarmLNode
		Type	Local
		At	LarmL
	
	End

	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		part	:BodyAura02.part
		Pmagnet LarmLNode
		POther  LarmLNode
		
		
		
	End

End

Condition
	On	Time
	Time	0
	

	Event	
		ENAME	LarmLNode2
		Type	Local
		At	LarmL
	
	End

	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:BodyAura02.part
		Pmagnet LarmLNode2
		POther  LarmLNode2
			
	End

	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	HandL
		part	:BodyAura02.part		
	End

	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	HandR
		part	:BodyAura02.part		
	End
End

Condition
	On	Time
	Time	0
	

	Event	
		ENAME	LleglNode
		Type	Local
		At	Llegl
	
	End

	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part	:LegAura02.part
		Pmagnet LleglNode
		POther  LleglNode
		
		
		
	End
End



Condition
	On	Time
	Time	0
	
	Event	
		ENAME	LlegRNode
		Type	Local
		At	LlegR
	End

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part	:LegAura02.part
		Pmagnet LlegRNode
		POther  LlegRNode
			
	End
End

Condition
	On	Time 
	Time	0
	
	Event	 
		Type	Local
		At	LlegR
		part	:BodyAura02.part
		Pmagnet FootR
		POther  FootR
	End

	Event	 
		Type	Local
		At	LlegL
		part	:BodyAura02.part
		Pmagnet FootL
		POther  FootL
	End

End



End