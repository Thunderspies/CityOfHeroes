#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

Input  
	InpName	Origin
End

Input  
	InpName	Target
End    

Input  
	Inpname	WepR
End

####################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Hand1
		Type	Local 
		At	WepR
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepR
		Sound radiation3_loop 80 80 .34
		LifeSpan	60
	End

End

Condition
	On 	Time
	Time 	19
	Event
		EName	Handburst
		Type	Local 
		At	WepR
		Part1	:RadiationLightHitBurst2.part
		Part2	:BurstRings.Part
		
	End

End

Condition
	On 	Time
	Time 	17
	
	Event
		EName 	Prime
		Type	Start 
		At	WepR
		Bhvr	behaviors/Fastprojectile3.bhvr
		Magnet	Target
		LookAt	Target
		Part1	:LightHead.part
		Part2	:LightHead.part
		Part3	:LightHead.part
		Part4	:LightHead.part
		Part5	:LightHead.part
		Sound neutrino1 60 60 .83
	End

	Event
		EName 	streak
		Type	Start 
		At	WepR
		Bhvr	behaviors/Fastprojectile3.bhvr
		Magnet	Target
		LookAt	Target
		Part1	:LightHead.part
		Part2	:RadiationHandElectronsLocal2.part
				
	End

	Event
		EName 	Head
		Type	Start 
		At	WepR
		Bhvr	behaviors/Fastprojectile3.bhvr
		Magnet	Target
		LookAt	Target
		Part1	:BlastStreakGlowHead.part
		Part2	:RadiationHandElectronsLocal2.part
		Part3	:InfectBubbleRingssHandsStreak.part
		Part5	:InfectBubbleDotsHandsStreak.part
		
	End

End

Condition
	On	time
	Time	25
		
	Event
		Ename	HandBurst
		Type	Destroy
	End

End

Condition
	On	PrimeHit
		
	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		Ename	streak
		Type	Destroy
	End

	Event
		Ename	Head
		Type	Destroy
	End
	
End

End	


			