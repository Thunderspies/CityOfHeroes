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
	Inpname	WepL
End

Input  
	Inpname	WepR
End
#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Electrons
		Type	local 
		At	WepL
		Part1	:RadiationBlastHandRings.part
		Part2	:RadiationBlastHandBalls.part
	End

	Event
		EName	Hand1
		Type	Local 
		At	WepR
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepR
		Sound radiation3_loop 80 80 .4
		LifeSpan	100
	End

	Event
		EName	Hand2
		Type	Local 
		At	WepL
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepL
		LifeSpan	100
	End

End


Condition
	On 	Time
	Time 	5

	Event
		EName	InitialSwordFire
		Type	Posit 
		At	WepL
		Part1	:RadiationBlastHands.part
		#Part2	:RadiationHandElectrons.part
	End

End


Condition
	On 	Time
	Time 	18.5

	Event
		
		Type	Posit 
		At	WepL
		Part5	:RadiationBlastHandsBurst.part
	End

End


Condition
	On 	Time
	Time 	20

	Event
		EName	InitialSwordFire
		Type	Destroy
		
	
	End

End

Condition
	On 	Time
	Time 	26

	Event
		EName	Electrons
		Type	Destroy
		
	
	End

End

Condition
	On 	Time
	Time 	18

	Event
		EName 	Prime
		Type	Start 
		At	Origin
		Bhvr	behaviors/Medianprojectile1.bhvr
		Magnet	Target
		LookAt	Target
		Part1	:RadiationBlastHead.part
		Part2	:RadiationBlastTail.part
		Part3	:RadiationBlastStreak.part
		Part4	:RadiationBlastFogOvershoot.part
		Part5	:RadiationBlastGlow.part
		
	End


End

Condition
	On	PrimeHit
		
	Event
		Ename	Prime
		Type	Destroy
	End

	Event	
		Ename	LightRays
		Type	Posit
		At	target
		part1	:RadiationLightHit.part
		
	
	End

	Event	

		Ename	LightHit
		Type	Posit
		At	target
		part1	:IlnessRingsHit.part
		part3	:IlnessBubbleHit.part
		part2	:RadiationCloudHit2.part
	End

End

Condition
	On 	Time
	Time 	110
	Event
		EName	LightHit
		Type	Destroy
		
	
	End


End

End	


			