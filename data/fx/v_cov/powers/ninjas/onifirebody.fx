#########################################################
##	template

FxInfo


#########################################################
####################### AUDIO ###########################
#########################################################




#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	0

##########################################################################3


	Event	
		ENAME	EyeNodeL
		Type	Local
		At	UarmL
		Part1	:OniEyes_Flash.part
		Part2	:OniEyes_Yellow.part
		Bhvr	:SpadEyeNodeL.bhvr
	End

	Event	
		ENAME	EyeNodeR
		Type	Local
		At	UarmL
		
		Part1	:OniEyes_Flash.part
		Part2	:OniEyes_Yellow.part
		Bhvr	:SpadEyeNodeR.bhvr
	End


	Event	
		ENAME	EyeNodeL
		Type	Local
		At	UarmR
		Part1	:OniEyes_Flash.part
		Part2	:OniEyes_Yellow.part
		Bhvr	:SpadEyeNodeLa.bhvr
	End

	Event	
		ENAME	EyeNodeR
		Type	Local
		At	UarmR
		
		Part1	:OniEyes_Flash.part
		Part2	:OniEyes_Yellow.part
		Bhvr	:SpadEyeNodeRa.bhvr
	End
####################################################################################################

	Event	
		ENAME	HeadNode
		Type	Local
		At	Head
		Bhvr	:HeadOffset1.bhvr
	End

	Event	
		Type	Local
		At	UarmR
		part	:Fiery01.part
		Pmagnet UarmR
	End

	Event	
		Type	Local
		At	FootR
		part	:Fiery01.part
		Pmagnet FootR
	End

	Event	
		Type	Local
		At	FootL
		part	:Fiery01.part
		Pmagnet FootL
	End


	Event	
		Type	Local
		At	UarmL
		part	:Fiery01.part
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
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		part	:FireCrawlBody01.part
		part	:FireBaseBody01.part

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
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		part	:FireCrawlBody01.part
		part	:FireBaseBody01.part
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
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		part	:FireCrawlBody01.part
		part	:FireBaseBody01.part
		Pmagnet SpadRNode
		POther  SpadRNode
		
		
	
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
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		part	:FireCrawlBody01.part
		part	:FireBaseBody01.part
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
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		part	:FireCrawlBody01.part
		part	:FireBaseBody01.part
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
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		part	:FireCrawlBody01.part
		part	:FireBaseBody01.part
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
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		part	:FireCrawlBody01.part
		part	:FireBaseBody01.part
		Pmagnet LarmLNode2
		POther  LarmLNode2
		
		
		
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
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		part	:FireCrawlBody01.part
		part	:FireBaseBody01.part
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
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		part	:FireCrawlBody01.part
		part	:FireBaseBody01.part
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
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		part	:FireCrawlBody01.part
		part	:FireBaseBody01.part
		Pmagnet FootR
		POther  FootR
	End

	Event	 
		Type	Local
		At	LlegL
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		part	:FireCrawlBody01.part
		part	:FireBaseBody01.part
		Pmagnet FootL
		POther  FootL
	End

End



End