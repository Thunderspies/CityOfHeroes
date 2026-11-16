#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	300


#########################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Hand1
		Type	Local 
		At	UarmR
		Part1	:DarkBlastTenticle.part
		Part3	:DarkHandsEmberb.part
				
		PMagnet	LarmR
		LifeSpan	20
		Sound DarkBlast1 80 80 .77

	End

	Event
		EName 	Hand2
		Type	Local 
		At	LarmR
		Part1	:DarkBlastTenticle.part
		Part3	:DarkHandsEmberb.part
		
		PMagnet	WepR
		LifeSpan	20
	End
	
End


Condition
	On 	Time
	Time 	22

	
	Event
		EName 	Prime
		Type	Start 
		At	WepR
		
		Part1	:DarkBlastPuddleTrail_Subtractive.Part
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

Condition
	On 	Time
	Time	70

	Event
		EName 	HitFake
		Type	Destroy

	End
End

Condition
	On 	Time
	Time	50

	Event
		EName 	Hand1
		Type	Destroy

	End

	Event
		EName 	Hand2
		Type	Destroy

	End

	Event
		EName 	Hand3
		Type	Destroy

	End

	Event
		EName 	Hand4
		Type	Destroy

	End

End


End	


			