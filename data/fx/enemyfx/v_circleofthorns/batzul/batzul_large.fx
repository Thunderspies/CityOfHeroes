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
		Part1	:footFlame.part
		Part2	:footFlame2.part
		Part3	:BatZulspark.part
		Part4	:BatZulFlame.part
		part	:FireEyesRight01.part
		part	:FireEyesLeft01.part
		
	End
	
	Event
		EName 	fire1
		Type	Local
		At	FootR

		BHVR	Behaviors/FootOffset.bhvr
		Part1	:footFlame.part
		Part2	:footFlame2.part
		Part3	:BatZulspark.part
		Part4	:BatZulFlame.part
		part	:FireEyesRight01.part
		part	:FireEyesLeft01.part
		
	End

	Event
		EName 	fire1
		Type	Local
		At	Col_R
		
		Part1	:BatZulFlame1.part
		Part2	:BatZulspark.part
		#PMagnet	UarmR
	End
	
	Event
		EName 	fire2
		Type	Local
		At	Col_L
		
		Part1	:BatZulFlame.part
		Part2	:BatZulspark.part
		#PMagnet	UarmL
	End

	Event
		EName 	fire2
		Type	Local
		At	Back
		
		Part1	:BatZulFlameBack.part
		Part2	:BatZulspark.part
		
	End
	
	Event
		EName 	fire2
		Type	Local
		At	Neck
		
		Part1	:BatZulFlameNeck.part
		Part2	:BatZulspark.part
		
	End

	Event
		EName 	fire2
		Type	Local
		At	UarmR
		
		Part1	:BatZulFlameArm1.part
		Part2	:BatZulsparkArm.part
		
	End

	Event
		EName 	fire2
		Type	Local
		At	UarmL
		
		Part1	:BatZulFlameArm.part
		Part2	:BatZulsparkArm.part
		
	End

	Event
		EName 	fire2
		Type	Local
		At	UarmR

		Part1	:BatZulUpperFlameArm.part
		PMagnet	LarmR
	End

	Event
		EName 	fire2
		Type	Local
		At	UarmL
		
		Part1	:BatZulUpperFlameArm.part
		PMagnet	LarmL
	End

	Event
		EName 	fire2
		Type	Local
		At	LarmR
		
		Part1	:BatZulLowerFlameArm1.part
		Part2	:BatZulsparkArm.part
		PMagnet	UarmR
	End

	Event
		EName 	fire2
		Type	Local
		At	LarmL
		
		Part1	:BatZulLowerFlameArm1.part
		Part2	:BatZulsparkArm.part
		PMagnet	UarmL
	End

	Event
		EName 	fire2
		Type	Local
		At	WepR
		
		Part1	:BatZulLowerFlameArm1a.part
		Part2	:BatZulsparkArm.part

		PMagnet	LarmR
	End

	Event
		EName 	fire2
		Type	Local
		At	WepL
		
		Part1	:BatZulLowerFlameArm1a.part
		Part2	:BatZulsparkArm.part
		PMagnet	LarmL
	End

		Event	
		Type	Local
		At	WepR
		part	:Fiery01.part
		part	:FieryFist01.part
		Pmagnet HandR
	End

	Event	
		Type	Local
		At	WepL
		part	:Fiery01.part
		part	:FieryFist01.part
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
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
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
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		part	:FireCrawlBody01.part
		part	:FireBaseBody01.part
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
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
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
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		part	:FireCrawlBody01.part
		part	:FireBaseBody01.part
		Pmagnet LarmLNode2
		POther  LarmLNode2	
		
	End

	Event	
		Type	Local
		At	Back
		part	:FireFlameBody01.part
		
	End

	Event	
		Type	Local
		At	Hips
		part	:FireFlameBody01.part
		Part	:BatZulFlame1.part
		Part	:BatZulFlame.part
		
	End

	Event	
		Type	Local
		At	Eyes
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		part	:FireFlameBody01.part
		
	End

	Event	
		Type	Local
		At	Back
		part	:FireFlameBody01.part
		part	:FireCrawlBody01.part
		part	:FireEmbersBody01.part
	
		
	End

	Event
		Type	Local
		At	WepR
		
		Part1	:BatZulLowerFlameArm1a.part
		Part2	:BatZulsparkArm.part
		part	:FireEyesLeft01.part
		part	:FireEyesLeft01.part
	End

	Event
		Type	Local
		At	WepL
		
		Part1	:BatZulLowerFlameArm1a.part
		Part2	:BatZulsparkArm.part
		part	:FireEyesRight01.part
		part	:FireEyesRight01.part
	End

	Event	
		Type	Local
		At	HandR
		part	:FireFlameBody01.part
		BHVR	:HandNode_R.bhvr
		
	End

	Event	
		Type	Local
		At	HandL
		part	:FireFlameBody01.part
		BHVR	:HandNode_L.bhvr
		
	End

End

End

