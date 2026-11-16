#########################################################
##	Inferno FX
##
FxInfo
	
lifespan	90

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

Condition

	On Time 
	Time 0
	
	Event
		Type	Local 
		At	hips
	
	End

	Event
		EName	Hand1
		Type	Local 
		At	WepR
		Part1	:RadiationInfectOthersHands.part
		
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepR
		Sound radiation3_loop 80 80 .35
		LifeSpan	45
	End

	Event
		EName	Hand1
		Type	Local 
		At	WepR
	
		Part2	:RadiationHandElectrons.part
		PMagnet	WepR
		
		LifeSpan	30
	End

	Event
		EName	Hand2
		Type	Local 
		At	WepL
		Part1	:RadiationInfectOthersHands.part
		
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepL
		Sound mutationthrow 66 66 .75
		LifeSpan	45
	End

	Event
		EName	Hand2
		Type	Local 
		At	WepL
		
		Part2	:RadiationHandElectrons.part
		PMagnet	WepL
		
		LifeSpan	30
	End

	Event
		EName 	Hookup
		Type	Local 
		At	root
		Geom	RootToChestAdjustment
		
	End
	
End

Condition

	On 	Time
	Time 	20

	Event
		EName   RadiusFxScale
		Type	local
		At	hookup
		altpiv	1
		Bhvr	powers/firecontrol/Inferno.bhvr
	End


	Event
		EName 	Prime1
		Type	Local 
		At	RadiusFxScale
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		Part1	:IrradiateYellow2.part
		Part2	:IrradiateYellowFlame01.part
		Part3	:IrradiateRings.part
		Part4	:IrradiateGlows.part
		
		
	End		


End

Condition
	On 	Time
	Time 	15
		
	Event
		Type	Local
		At 	Root
		Sound Nova2 100.0 100.0 .8
	End
End

Condition

	On 	Time
	Time 	15


	Event
		EName 	rays
		Type	Local 
		At	hips		
		
		Part3	:EMPSpark.part

	End

End

Condition
	On 	Time
	Time 	18
	Event
		EName 	Prime8
		Type	Local 
		At	Hookup
		Altpiv	1
		Part1   :Irradiateflash.part

	End

End

Condition
	On 	Time
	Time	45

	Event
		EName 	rays
		Type	Destroy
	End

End

Condition
	On 	Time
	Time	47

	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		EName 	Primex
		Type	Destroy
	End

End

Condition
	On 	Time
	Time	50

	Event
		EName 	Prime1
		Type	Destroy
	End

End	

End
