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
	Inpname	Root
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Hand2
		Type	Local 
		At	WepL
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepL
		Sound radiation3_loop 80 80 .33
		LifeSpan	70
	End
End

Condition
	On 	Time
	Time 	30
	Event
		EName	Handburst
		Type	Local 
		At	WepL
		Part1	:RadiationLightHitBurst.part
		Part2	:BurstRings.Part
		LifeSpan	12
	End

End

Condition
	On 	Time
	Time 	35

	Event
		EName 	Prime
		Type	Start 
		At	wepL
		Bhvr	behaviors/CosmicBlast.bhvr
		Geom	FX_Locknar
		Magnet	Target
		LookAt	Target
		Part3	:NeutrinoStreak.part
		
	End

	Event
		EName	InitialSwordFire
		Type	Local 
		At	Prime
		Part1	:RadiationInfectOthersHandsLocal.part
		Part2	:RadiationHandElectronsLocal.part
		Part3	:InfectBubbleDotsHandsLocal.part
		Part4	:InfectBubblesHandsLocal.part
		Sound radburst 80 80 .77
		
	End

	Event
		EName	InitialSwordFire
		Type	Local 
		At	Prime
		#Part1	:RadiationInfectOthersHands.part
		#Part2	:RadiationHandElectronsLocal.part
		Part3	:InfectBubbleDotsHandsStreak.part
		Part4	:InfectBubblesHandsStreak.part
		
	End
End

Condition
	On	Time
	Time	35
		
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

End


End	


			