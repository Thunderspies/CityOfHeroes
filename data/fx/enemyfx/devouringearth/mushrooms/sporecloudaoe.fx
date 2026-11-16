#########################################################
##	Inferno FX
##
FxInfo

Input  
	InpName	Root
End

Input  
	Inpname	hips
End

Input  
	Inpname	WepR
End

Input  
	Inpname	WepL
End

Flags    InheritAlpha

LifeSpan	35

Condition
	On Time
	Time 24

	Event
		Type	Local
		At	hips
		Sound mushroomburst2 80 80 .99
	End
End

Condition
	On Time
	Time 0

	Event
		Type	Local
		At	hips
		Sound Nova3 80 80 .77
	End
End

Condition

	On Time 
	Time 5
	
	Event
		Type	Local 
		At	hips
		
	End

	Event
		EName	Hand1
		Type	Local 
		At	WepR
		#Part1	:RadiationInfectOthersHands.part
		#Part2	:RadiationHandElectrons.part
		#Part3	:InfectBubbleDotsHands.part
		#Part4	:InfectBubblesHands.part
		PMagnet	WepR
		
	
		LifeSpan	30
	End


	Event
		EName	Hand2
		Type	Local 
		At	WepL
		#Part1	:RadiationInfectOthersHands.part
		#Part2	:RadiationHandElectrons.part
		#Part3	:InfectBubbleDotsHands.part
		#Part4	:InfectBubblesHands.part
		PMagnet	WepL
		LifeSpan	30
	End

End

#####################################################################################

Condition

	On 	time
	Time 	23 ##37
	
	Event
		EName   RadiusFxScale
		Type	local
		At	Root
		Bhvr	Behaviors/Spores.bhvr
		LifeSpan	30
	End

	Event
		EName 	Prime
		Type	Local 
		At	RadiusFxScale
		bhvr    behaviors/SporeRingScale.bhvr
		Part1	:SporeCloud01.part
		Part2	:SporeCloud02.part
		
		Part4	:SporeGlows.part
		
		LifeSpan	35
	End		

	Event
		EName 	Prime1
		Type	Local 
		At	RadiusFxScale
		bhvr    behaviors/SporeRingScaleSlower.bhvr
	
		Part3	:SporeDust.part
		Part	:SporeDustFlat.part
		
		LifeSpan	35
	End

End


End
