#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	200

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
		Sound tentacleattack2 80 80 .76
				
		PMagnet	LarmR
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
	Time 	20

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
		Bhvr	behaviors/MedianProjectile5.bhvr
		
		Part	:TendrillClouds_Subtractive.part
		Part	:TendrillClouds2_Subtractive.part

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


			