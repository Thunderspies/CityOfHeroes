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

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Hand1
		Type	Local 
		At	UarmR
		Part1	:DarkBlastTenticleWhite.part
		Sound Darkmoonbeam3b 80 80 .8
		Part3	:DarkHandsEmberb.part
		
		
		
		PMagnet	LarmR
	End

End

Condition
	On 	Time
	Time 	15

	Event
		EName 	Hand2
		Type	Local 
		At	LarmR
		Part1	:DarkBlastTenticleWhite.part
		
		Part3	:DarkHandsEmberb.part
		
		PMagnet	WepR
	End

End

#########################################################################################################

Condition
	On 	Time
	Time 	5

	
	Event
		EName 	Prime
		Type	Start 
		At	WepR

		Bhvr	behaviors/slowprjct5.bhvr
		Magnet	Target
		LookAt	Target
		
		Part	:DarkBlastPuddleTrail_Subtractive.Part
		Part5	:BeamRapidTenticles.part
		Part1	:DarkBlastPuddleTrail2.Part
		Part1	:MoonBeamLightRays.part
		
		Part3	:MoonBeamEmber2.part
		Part4	:MoonBeamEmberLarge2.part
								
	End

	Event
		EName 	Prime2
		Type	Start 
		At	WepR
		Part1	:BeamRapidTenticlesHead.part
		Bhvr	behaviors/slowprjct5.bhvr
		Magnet	Target
		LookAt	Target
		Part1	:MoonBeamRapidCloud2.part
		#Part1	:MoonBeamLightRays.part
		#Part2	:MoonBeamBlastDark.part
		Part2	:MoonBeamRapidCloud.part
		Part3	:DarkBlastTendrill01a.part
		Part4	:DarkBlastTendrill03a.part
				
	End

	Event
		EName 	Prime3
		Type	Start 
		At	WepR

		Bhvr	behaviors/slowprjct5.bhvr
		Magnet	Target
		LookAt	Target
		
		#Part1	:MoonBeamBlast.part
		#Part2	:MoonBeamBlastDark.part

		Part3	:DarkBlastTendrill02a.part
		Part4	:DarkBlastTendrill04a.part
						
	End


End


Condition
	On 	PrimeHit

	Event
		EName 	HitFake
		Type	local 
		At	target
		#Part1	:WhiteBlast.part
		#Part2	:WhiteBlast.part
						
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
	Time	50

	Event
		EName 	HitFake
		Type	Destroy

	End
End

Condition
	On 	Time
	Time	45

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


			