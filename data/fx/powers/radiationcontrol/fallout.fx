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
		Sound Nova2 100.0 100.0 .8
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
		Sound radiation3_loop 80 80 .35
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
	Time 	30

	Event
		EName 	Prime
		Type	Local 
		At	hips
		Part1	:IrradiateRingsHips.part
		
		Part3	:IrradiateGlowHips.part
		Lifespan	48
	End

	Event
		EName 	Primex
		Type	Local 
		At	hips

		Part1	:IrradiaterockBits.part
		Part2	:IrradiaterockBits2.part
		Lifespan	48
	End



	Event
		EName   RadiusFxScale
		Type	local
		At	Root
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
	Time 	25

	Event
		EName 	Prime
		Type	Local 
		At	hips
		
		Part2	:Irradiate.part
	

	End

End

Condition
	On 	Time
	Time 	28
	Event
		EName 	Prime8
		Type	Local 
		At	root
		Part1   :Irradiateflash.part
		Part2   :Irradiateflash2.part
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
	Time	60

	Event
		EName 	Prime1
		Type	Destroy
	End

End	

End
