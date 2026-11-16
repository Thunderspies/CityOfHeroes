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

Input  
	Inpname	Root
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
		Part3	:Behemothspark.part
		
	End
	
	Event
		EName 	fire1
		Type	Local
		At	FootR
		
		BHVR	Behaviors/FootOffset.bhvr
		Part1	:footFlame.part
		Part2	:footFlame2.part
		Part3	:Behemothspark.part
		
	End

	Event
		EName 	fire1
		Type	Local
		At	Col_R
		
		Part1	:BehemothFlame1.part
		Part2	:Behemothspark.part
		#PMagnet	UarmR
	End
	
	Event
		EName 	fire2
		Type	Local
		At	Col_L
		
		Part1	:BehemothFlame.part
		Part2	:Behemothspark.part
		#PMagnet	UarmL
	End

	Event
		EName 	fire2
		Type	Local
		At	Back
		
		Part1	:BehemothFlameBack.part
		Part2	:Behemothspark.part
		
	End
	
	Event
		EName 	fire2
		Type	Local
		At	Neck
		
		Part1	:BehemothFlameNeck.part
		Part2	:Behemothspark.part
		
	End

	Event
		EName 	fire2
		Type	Local
		At	UarmR
		
		Part1	:BehemothFlameArm1.part
		Part2	:BehemothsparkArm.part
		
	End

	Event
		EName 	fire2
		Type	Local
		At	UarmL
		
		Part1	:BehemothFlameArm.part
		Part2	:BehemothsparkArm.part
		
	End

	Event
		EName 	fire2
		Type	Local
		At	UarmR
		
		Part1	:BehemothUpperFlameArm1.part
		#PMagnet	LarmR
	End

	Event
		EName 	fire2
		Type	Local
		At	UarmL
		
		Part1	:BehemothUpperFlameArm.part
		#PMagnet	LarmL
	End

	Event
		EName 	fire2
		Type	Local
		At	LarmR
		
		Part1	:BehemothLowerFlameArm1.part
		Part2	:BehemothsparkArm.part
		PMagnet	UarmR
	End

	Event
		EName 	fire2
		Type	Local
		At	LarmL
		
		Part1	:BehemothLowerFlameArm.part
		Part2	:BehemothsparkArm.part
		PMagnet	UarmL
	End


	Event
		EName 	fire2
		Type	Local
		At	WepR
		
		Part1	:BehemothLowerFlameArm1a.part
		Part2	:BehemothsparkArm.part
		PMagnet	LarmR
	End

	Event
		EName 	fire2
		Type	Local
		At	WepL
		
		Part1	:BehemothLowerFlameArma.part
		Part2	:BehemothsparkArm.part
		PMagnet	LarmL
	End

########################################################
#Eyes
########################################################	
	
	Event
		EName 	hookup
		Type	Local
		At	Head
		Geom	Minion_Eyey	
		
	End

	Event
		EName 	headhook1
		Type	Local
		At	hookup
		altpiv	1
		part1	:DaemonEyes1.part
		part2	:DaemonPupil1.part
		Part3	:BehemothEyesMistLeft.part
	End

	Event
		EName 	headhook2
		Type	Local
		At	hookup
		altpiv	2
		part1	:DaemonEyes1.part
		part2	:DaemonPupil1.part
		Part3	:BehemothEyesMistRight.part
	End

	

End

########################################################
#Snort Holes
########################################################

Condition
	On 	cycle
	Time	100
	Chance	1

	Event
		EName 	Prime
		Type	Local 
		At	hookup
		altpiv	4
		part1	:SnortSmokeRight.part
		
		lifespan	25
	End

	Event
		EName 	headhook
		Type	Local
		At	hookup
		altpiv	5
		part1	:SnortSmokeLeft.part
		
		lifespan	25
	End

End


End
##################################

