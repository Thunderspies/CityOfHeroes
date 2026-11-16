#########################################################
##	Inferno FX
##
FxInfo
	
lifespan	150

Input  
	InpName	UlegR
End

Input  
	InpName	LlegR
End

Input  
	Inpname	FootR
End

Input  
	InpName	Root
End

Input  
	Inpname	mystic
End

Condition

	On Time 
	Time 0
	
	Event
		Type	Local 
		At	UlegR
		Sound IceNova2 100.0 100.0 .88
		Part1	:ColdArmMist.part
		Part2	:FrostHands.part
		Part3	:snowburst.part
		LifeSpan	30
	End

	Event
		Type	Local 
		At	LlegR
		Part1	:ColdArmMist.part
		Part2	:FrostHands.part
		Part3	:snowburst.part
		LifeSpan	30
	End

	Event
		Type	Local 
		At	FootR
		Part1	:ColdArmMist.part
		Part2	:FrostHands.part
		Part3	:snowburst.part
		LifeSpan	30
	End

End

Condition
	On 	Time
	Time 	25

	Event
		EName 	Prime
		Type	Local 
		At	mystic
		
		Bhvr	Behaviors/CameraShake01.bhvr
		#Part2	:Inferno.part

	End

	Event
		EName   RadiusFxScale
		Type	local
		At	mystic
		Bhvr	powers/firecontrol/Inferno.bhvr
	End

	Event
		EName 	Prime1
		Type	Local 
		At	RadiusFxScale
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		Part1	:ColdMistAura.part
		
		Part	:swordFlame01.part
		Part	:SwordFlame02.part
		Part	:IceCrystals.part
		Part	:IceCrystals2.part
		Part	:SnowFlakes.part
	End
	
	Event
		EName 	Prime1
		Type	Local 
		At	RadiusFxScale
		bhvr    behaviors/powers/firecontrol/Infernobubble2.bhvr
		Part1	:IceAuraSnow.part	
		
	End

	Event
		EName 	Prime1
		Type	Local 
		At	RadiusFxScale
		bhvr    behaviors/powers/firecontrol/Infernobubble3.bhvr
			
		Part2	:IceAuraBits.part
	End

	Event
		EName	ring
		Type	start
		At	RadiusFxScale
		geom	MegaRing2
		bhvr	behaviors/IceAuraRing.bhvr
		LifeSpan	3

	End

End

Condition
	On 	Time
	Time	67

	Event
		EName 	Prime
		Type	Destroy
	End

	
End
	

End
