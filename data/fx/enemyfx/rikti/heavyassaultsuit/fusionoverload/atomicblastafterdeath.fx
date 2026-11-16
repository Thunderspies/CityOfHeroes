#########################################################
##	RadiationAura
FxInfo

LifeSpan	190

Input  
	InpName	T_Chest
End

Input  
	Inpname	WepL
End

Input  
	Inpname	WepR
End

Input  
	Inpname	Origin
End

####################################################################


Condition
	On 	Time
	Time 	15

	Event
		EName	Electrons
		Type	local 
		At	WepL
		Part1	:RadiationBlastHandRings.part
		Part2	:RadiationBlastHandBalls.part
		Sound blackhole3 80 80 .75
		LifeSpan	80
	End

	Event
		EName	Electrons
		Type	local 
		At	WepR
		Part1	:RadiationBlastHandRings.part
		Part2	:RadiationBlastHandBalls.part
		Sound blackhole3 80 80 .75
		LifeSpan	80
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
		Sound radiation3_loop 80 80 .3
		LifeSpan	80
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
		LifeSpan	80
	End

End

Condition
	On	Time
	Time	100

	Event	
		Ename	LightRays
		Type	Posit
		At	T_Chest
		BHVR	Behaviors/genericParticleFade.bhvr
		part1	:RadiationLightHit.part
		Part2	:BurstRingsContinuing.Part
	End

	
End

End

