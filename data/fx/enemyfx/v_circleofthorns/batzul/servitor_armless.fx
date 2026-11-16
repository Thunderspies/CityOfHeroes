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
		Part1	:footFlame_small.part
		Part2	:footFlame2_small.part
		Part3	:BatZulspark_small.part
		#Part4	:BatZulFlame_small.part
		#part	:FireEyesRight01.part
		part	:FireEyesLeft01.part
		
	End
	
	Event
		EName 	fire1
		Type	Local
		At	FootR

		BHVR	Behaviors/FootOffset.bhvr
		Part1	:footFlame_small.part
		Part2	:footFlame2_small.part
		Part3	:BatZulspark_small.part
		#Part4	:BatZulFlame_small.part
		#part	:FireEyesRight01.part
		part	:FireEyesLeft01.part
		
	End

	Event
		EName 	fire1
		Type	Local
		At	Col_R
		
		Part1	:BatZulFlame_small.part
		Part2	:BatZulspark_small.part
		#PMagnet	UarmR
	End
	
	Event
		EName 	fire2
		Type	Local
		At	Col_L
		
		Part1	:BatZulFlame_small.part
		Part2	:BatZulspark_small.part
		#PMagnet	UarmL
	End

	Event
		EName 	fire2
		Type	Local
		At	Back
		
		Part1	:BatZulFlameBack_small.part
		Part2	:BatZulspark_small.part
		
	End
	
	Event
		EName 	fire2
		Type	Local
		At	Neck
		
		Part1	:BatZulFlameNeck_small.part
		Part2	:BatZulspark_small.part
		
	End

	Event	
		Type	Local
		At	Back
		part	:FireFlameBody01_small.part
		Part1	:BatZulFlame_small.part
		
	End

	Event	
		Type	Local
		At	Waist
		part	:FireFlameBody01_small.part
		Part1	:BatZulFlame_small.part
		
	End

	Event	
		Type	Local
		At	Hips
		part	:FireFlameBody01_small.part
		Part1	:BatZulFlame_small.part
		
	End

	Event	
		Type	Local
		At	Back
		part	:FireFlameBody01_small.part
		part	:FireCrawlBody01_small.part
		part	:FireEmbersBody01_small.part
	
	End

	Event	
		Type	Local
		At	Neck
		part	:FireFlameBody01_small.part
		Part1	:BatZulFlame_small.part
		part	:FireFlameBody01_Small_Servitor.part
		
	End
End

End

