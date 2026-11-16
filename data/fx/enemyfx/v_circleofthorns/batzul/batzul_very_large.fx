#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	head
End

Input  
	Inpname	Col_R
End

Input  
	Inpname	Col_L
End

Input  
	Inpname	UarmR
End

Input  
	Inpname	UarmL
End

Input  
	Inpname	LarmR
End

Input  
	Inpname	LarmL
End

Input  
	Inpname	Back
End

Input  
	Inpname	Neck
End

Input  
	Inpname	FootL
End

Input  
	Inpname	FootR
End

Input  
	Inpname	WepL
End

Input  
	Inpname	WepR
End



Flags    InheritAlpha


Condition
	On 	Time
	Time 	0
########################################################
#Fire&Sparks
########################################################

	Event
		EName 	hookup1
		Type	Local
		At	FootL
		#Geom	FootHookup	
		Sound firesword_loop 70 70 .67
		
	End

#	Event
#		EName 	hookup2
#		Type	Local
#		At	FootR
#		Geom	FootHookup	
#		
#	End

	Event
		EName 	fire1
		Type	Local
		At	FootL
		
		BHVR	Behaviors/FootOffset.bhvr
		Part1	:footFlame_large.part
		Part2	:footFlame2_large.part
		Part3	:BatZulspark_large.part
		Part4	:BatZulFlame_large.part
		part	:FireEyesRight01_large.part
		part	:FireEyesLeft01_large.part
		
	End
	
	Event
		EName 	fire1
		Type	Local
		At	FootR

		BHVR	Behaviors/FootOffset.bhvr
		Part1	:footFlame_large.part
		Part2	:footFlame2_large.part
		Part3	:BatZulspark_large.part
		Part4	:BatZulFlame_large.part
		part	:FireEyesRight01_large.part
		part	:FireEyesLeft01_large.part
		
	End

	Event
		EName 	fire1
		Type	Local
		At	Col_R
		
		Part1	:BatZulFlame1_large.part
		Part2	:BatZulspark_large.part
		#PMagnet	UarmR
	End
	
	Event
		EName 	fire2
		Type	Local
		At	Col_L
		
		Part1	:BatZulFlame_large.part
		Part2	:BatZulspark_large.part
		#PMagnet	UarmL
	End

	Event
		EName 	fire2
		Type	Local
		At	Back
		
		Part1	:BatZulFlameBack_large.part
		Part2	:BatZulspark_large.part
		
	End
	
	Event
		EName 	fire2
		Type	Local
		At	Neck
		
		Part1	:BatZulFlameNeck_large.part
		Part2	:BatZulspark_large.part
		
	End

	Event
		EName 	fire2
		Type	Local
		At	UarmR
		
		Part1	:BatZulFlameArm1_large.part
		Part2	:BatZulsparkArm_large.part
		
	End

	Event
		EName 	fire2
		Type	Local
		At	UarmL
		
		Part1	:BatZulFlameArm_large.part
		Part2	:BatZulsparkArm_large.part
		
	End

	Event
		EName 	fire2
		Type	Local
		At	UarmR

		Part1	:BatZulUpperFlameArm_large.part
		PMagnet	LarmR
	End

	Event
		EName 	fire2
		Type	Local
		At	UarmL
		
		Part1	:BatZulUpperFlameArm_large.part
		PMagnet	LarmL
	End

	Event
		EName 	fire2
		Type	Local
		At	LarmR
		
		Part1	:BatZulLowerFlameArm1_large.part
		Part2	:BatZulsparkArm_large.part
		PMagnet	UarmR
	End

	Event
		EName 	fire2
		Type	Local
		At	LarmL
		
		Part1	:BatZulLowerFlameArm1_large.part
		Part2	:BatZulsparkArm_large.part
		PMagnet	UarmL
	End

	Event
		EName 	fire2
		Type	Local
		At	WepR
		
		Part1	:BatZulLowerFlameArm1a_large.part
		Part2	:BatZulsparkArm_large.part

		PMagnet	LarmR
	End

	Event
		EName 	fire2
		Type	Local
		At	WepL
		
		Part1	:BatZulLowerFlameArm1a_large.part
		Part2	:BatZulsparkArm_large.part
		PMagnet	LarmL
	End

		Event	
		Type	Local
		At	WepR
		part	:Fiery01_large.part
		part	:FieryFist01_large.part
		Pmagnet HandR
	End

	Event	
		Type	Local
		At	WepL
		part	:Fiery01_large.part
		part	:FieryFist01_large.part
		Pmagnet HandL
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
		part	:FireFlameBody01_large.part
		part	:FireEmbersBody01_large.part
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
		At	WepR
		part	:FireFlameBody01_large.part
		part	:FireEmbersBody01_large.part
		part	:FireCrawlBody01_large.part
		part	:FireBaseBody01_large.part
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
		part	:FireFlameBody01_large.part
		part	:FireEmbersBody01_large.part
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
		At	WepL
		part	:FireFlameBody01_large.part
		part	:FireEmbersBody01_large.part
		part	:FireCrawlBody01_large.part
		part	:FireBaseBody01_large.part
		Pmagnet LarmLNode2
		POther  LarmLNode2	
		
	End

	Event	
		Type	Local
		At	Back
		part	:FireFlameBody01_large.part
		
	End

	Event	
		Type	Local
		At	Hips
		#part	:FireFlameBody01_large.part
		Part	:BatZulFlame1_large.part
		Part	:BatZulFlame_large.part
		
	End

	Event	
		Type	Local
		At	Waist
		part	:FireFlameBody01_large.part
		Part	:BatZulFlame1_large.part
		Part	:BatZulFlame_large.part
		
	End

	Event	
		Type	Local
		At	Chest
		part	:FireFlameBody01_large.part
		Part	:BatZulFlame1_large.part
		Part	:BatZulFlame_large.part
		
	End


	Event	
		Type	Local
		At	Eyes
		part	:FireFlameBody01_large.part
		part	:FireEmbersBody01_large.part
		part	:FireFlameBody01_large.part
		
	End

	Event	
		Type	Local
		At	Back
		part	:FireFlameBody01_large.part
		part	:FireCrawlBody01_large.part
		part	:FireEmbersBody01_large.part
	
		
	End

	Event
		Type	Local
		At	WepR
		
		Part1	:BatZulLowerFlameArm1a_large.part
		Part2	:BatZulsparkArm_large.part
		part3	:FireEyesLeft01_large.part
		part4	:FireEyesLeft01_large.part
	End

	Event
		Type	Local
		At	WepL
		
		Part1	:BatZulLowerFlameArm1a_large.part
		Part2	:BatZulsparkArm_large.part
		part	:FireEyesRight01_large.part
		part	:FireEyesRight01_large.part
	End

	Event	
		Type	Local
		At	HandR
		part	:FireFlameBody01_large.part
		BHVR	:HandNode_R.bhvr
		
	End

	Event	
		Type	Local
		At	HandL
		part	:FireFlameBody01_large.part
		BHVR	:HandNode_L.bhvr
		
	End

End

End

