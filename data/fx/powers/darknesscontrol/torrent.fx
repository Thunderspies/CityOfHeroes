#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	200


Input  
	Inpname	WepL
End

Input  
	Inpname	UarmL
End

Input  
	Inpname	LarmL
End

Input  
	Inpname	LarmR
End

Input  
	Inpname	UarmR
End


Input  
	Inpname	WepR
End

Input  
	Inpname	Root
End

Input  
	Inpname	T_Root
End
#########################################################

Condition
	On 	Time
	Time 	11

	Event
		EName 	Hand1
		Type	Local 
		At	UarmR
		Part1	:DarkBlastTenticle.part
		#Part2	:DarkHandsB.part
		Part3	:DarkHandsEmberb.part
				
		PMagnet	LarmR
		Sound dctorrent 90 90 .85
		LifeSpan	25
	End

	Event
		EName 	Hand3
		Type	Local
		At	UarmL
		Part1	:DarkBlastTenticle.part
		#Part2	:DarkHandsB.part
		Part3	:DarkHandsEmberb.part
				
		PMagnet	LarmL
		LifeSpan	25
	End

	Event
		EName 	Hand2
		Type	Local 
		At	LarmR
		Part1	:DarkBlastTenticle.part
		#Part2	:DarkHands2.part
		Part3	:DarkHandsEmberb.part
		
		PMagnet	WepR
		LifeSpan	25
	End

	

	Event
		EName 	Hand4
		Type	Local
		At	LarmL
		Part1	:DarkBlastTenticle.part
		#Part2	:DarkHands2.part
		Part3	:DarkHandsEmberb.part
		
		PMagnet	WepL
		LifeSpan	25
	End

End

Condition
	On 	Time
	Time 	21

	Event
		EName 	p
		Type	Start 
		At	Root
		Geom	Torrent				
	End

	Event
		EName 	Prime
		Type	Start 
		At	p
		 Altpiv	1
		Bhvr	behaviors/MedianProjectile4.bhvr
		
#		Part	:TendrillClouds_Subtractive.part
#		Part	:TendrillClouds2_Subtractive.part

		Part5	:TendrilTenticlesColor.part
		Part1	:TendrilTenticles.part
		
		Part2	:TendrillClouds2.part
		Part3	:TendrillClouds.part
		
		Magnet	T_Root
		LookAt	T_Root
				
	End
End

	

Condition
	On	PrimeHit
		
	Event
		Ename	Prime
		Type	Destroy
	End

End


End	


			