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
		Part1	:DarkSoulBeamTenticle.part
		Sound Darkmoonbeam3b 80 80 .8
		Part3	POWERS/DarknessControl/DarkHandsEmberb.part
		
		
		
		PMagnet	LarmR
	End

End

Condition
	On 	Time
	Time 	5

	Event
		EName 	Hand2
		Type	Local 
		At	LarmR
		Part1	:DarkSoulBeamTenticle.part
		
		Part3	POWERS/DarknessControl/DarkHandsEmberb.part
		
		PMagnet	WepR
	End

End

Condition
	On	Time
	Time	0
	
	Event
		EName 	HandR 
		Type	local
		At	WepR
				
		Part	:soulFlash1.part
		
		Part	:soulFire2.part

		Lifespan	1
	End

	Event
		EName 	HandR 
		Type	local
		At	WepR
		Part	:soulGlow1.part
		Part	:soulSmoke1.part
		
		Lifespan	10
	End

End

#########################################################################################################

Condition
	On 	Time
	Time 	1

	
	Event
		EName 	Prime
		Type	Start 
		At	WepR

		Bhvr	behaviors/slowprjct5.bhvr
		Magnet	Target
		LookAt	Target
		
		Part5	POWERS/DarknessControl/BeamRapidTenticles.part
		#Part1	POWERS/DarknessControl/DarkBlastPuddleTrail2.Part
		Part1	:DarkSoulBeamLightRays.part
		
		Part3	POWERS/DarknessControl/MoonBeamEmber2.part
		Part4	POWERS/DarknessControl/MoonBeamEmberLarge2.part

		#Part1	POWERS/DarknessControl/BeamRapidTenticlesHead.part
		#Part1	POWERS/DarknessControl/MoonBeamRapidCloud2.part
		#Part1	POWERS/DarknessControl/MoonBeamLightRays.part
		#Part2	POWERS/DarknessControl/MoonBeamBlastDark.part
		Part2	POWERS/DarknessControl/MoonBeamRapidCloud.part
		Part3	:DarkBlastTendrill01a.part
		Part4	:DarkBlastTendrill03a.part
		
		#Part1	POWERS/DarknessControl/MoonBeamBlast.part
		#Part2	POWERS/DarknessControl/MoonBeamBlastDark.part

		Part3	:DarkBlastTendrill02a.part
		Part4	:DarkBlastTendrill04a.part

		
		Part	:soulSmoke1.part
		Part	:soulSmoke1.part

		Part	:soulGlow0.part
						
	End


End


Condition
	On 	PrimeHit

	Event
		EName 	HitFake
		Type	local 
		At	target
		#Part1	POWERS/DarknessControl/WhiteBlast.part
		#Part2	POWERS/DarknessControl/WhiteBlast.part
						
	End
	
	Event
		EName 	Prime
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
	Time	30

	Event
		EName 	Hand1
		Type	Destroy

	End

	Event
		EName 	Hand2
		Type	Destroy

	End


End


End	


			