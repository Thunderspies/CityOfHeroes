#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	300

Input  
	InpName	Origin
End

Input  
	InpName	Target
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
	Inpname	Eyes
End

Input  
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Hand1
		Type	Local 
		At	Eyes
		Part1	:DarkBlastTenticle.part
		Part3	:DarkHandsEmberb.part
				
		PMagnet	Hips
		LifeSpan	20
		Sound DarkBlast1 80 80 .77

	End

	Event
		EName 	Hand2
		Type	Local 
		At	Eyes
		Part1	:DarkBlastTenticle.part
		Part3	:DarkHandsEmberb.part
		
		PMagnet	Hips
		LifeSpan	20
	End
	
End


Condition
	On 	Time
	Time 	2

	
	Event
		EName 	Prime
		Type	Start 
		At	Hips

		
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
		At	Eyes

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
		At	Eyes

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
	Time	48

	Event
		EName 	HitFake
		Type	Destroy

	End
End

Condition
	On 	Time
	Time	28

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


			