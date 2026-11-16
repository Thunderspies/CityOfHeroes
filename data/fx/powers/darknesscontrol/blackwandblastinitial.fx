#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

######################################################################################3
##Audio
##########################################################3

Condition
	
	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepR

		Geom	Staff_06	##This geom needed##	
		
		#LifeSpan 45
	End

End


################################################################
	
Condition

	On	Time	
	Time 	50

	Event
		Type 	Local
		At	WepR
		Sound miasmaburst 100 100 .55
	End
End

Condition
	On	Time
	Time	50	#24

	Event
		EName 	Prime
		Type	Start 
		At	InitialSwordFire
		Altpiv	1
		
		Part2	:BeamRapidTenticles.part
		Part1	:DarkBlastPuddleTrail.Part
		Part3	:BeamRapidCloud2.part
		Part4	:MoonBeamEmber.part
		Part5	:MoonBeamEmberLarge.part
		Bhvr	behaviors/slowprjct5.bhvr
		Magnet	Target
		LookAt	Target
						
	End

	Event
		EName 	Prime2
		Type	Start 
		At	WepR

		Bhvr	behaviors/slowprjct5.bhvr
		Magnet	Target
		LookAt	Target
		Part1	:MoonBeamRapidCloud.part
		Part2	:BeamRapidTenticlesHead.part
		Part3	:DarkBlastTendrill01.part
			
		Part4	:DarkBlastTendrill03.part
				
	End

	Event
		EName 	Prime3
		Type	Start 
		At	WepR

		Bhvr	behaviors/slowprjct5.bhvr
		Magnet	Target
		LookAt	Target
		
		Part2	:DarkBlastTendrill02.part
		Part3	:DarkBlastTendrill04.part
						
	End


End

Condition
	On 	PrimeHit

	Event
		EName 	HitFake
		Type	local 
		At	target
		Part1	:Blast.part
		Part2	:Blast.part
						
	End
	
	Event
		EName 	Prime
		Type	Destroy

	End

	Event
		EName 	Prime2
		Type	Destroy

	End

	Event
		EName 	Prime3
		Type	Destroy

	End

End

End	


			