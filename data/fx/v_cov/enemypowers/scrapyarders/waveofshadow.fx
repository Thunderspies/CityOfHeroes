#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

Lifespan	90
#########################################################

Condition
	On 	Time
	Time 	11

	Event
		EName 	Hand1
		Type	Local 
		At	UarmR
		Part1	:DarkBlastTenticle.part
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
		Part3	:DarkHandsEmberb.part
				
		PMagnet	LarmL
		LifeSpan	25
	End

	Event
		EName 	Hand2
		Type	Local 
		At	LarmR
		Part1	:DarkBlastTenticle.part
		Part3	:DarkHandsEmberb.part
		
		PMagnet	WepR
		LifeSpan	25
	End

	

	Event
		EName 	Hand4
		Type	Local
		At	LarmL
		Part1	:DarkBlastTenticle.part
		Part3	:DarkHandsEmberb.part
		
		PMagnet	WepL
		LifeSpan	25
	End

End

Condition
	On 	Time
	Time 	21

	Event
		EName 	Prime
		Type	Start 
		At	Chest
		Geom	HelixFXOffset
		Bhvr	:ProjectileWaveCone.bhvr

		Magnet	T_Hips
	End

	Event
		EName 	cone
		Type	Start
		At	Prime		
		Part4	:WaveShadowCone.part
		
		LookAt	T_Hips	
	End

	Event
		EName 	core
		Type	Local
		At	Prime		
		Part	:WaveShadowCone2.part
		
		LookAt	T_Hips	
	End

	Event
		EName 	spiral
		Type	Local
		At	Prime
		 Altpiv	01
		
		Part5	:TendrilTenticlesColor.part
		Part1	:TendrilTenticles.part
		Part3	:TendrillClouds.part
		Part2	:TendrillClouds2.part
		
		LookAt	T_Hips
				
	End

End

	

Condition
	On	PrimeHit
		
	Event
		Ename	All
		Type	Destroy
	End

End


End	


			