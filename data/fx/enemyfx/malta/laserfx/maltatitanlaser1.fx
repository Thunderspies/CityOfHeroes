#########################################################
##	cot_waterfall_med.fx
##
FxInfo
	
Input  
	InpName	Origin
End


Input  
	InpName	Target
End


Input  
	InpName	mystic
End

Input  
	InpName	WepR
End

LifeSpan	200

###########################################################



Condition
	On	Time
	Time	18

	Event	
		Type	Local
		At	Origin
		Sound titanlaser3 140 140 1.0
	End

End

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	WepNode
		Type	local
		At	WepR
		Bhvr	:LaserOffset.bhvr
		Lifespan 100

	End

		
End

Condition
	On 	Time
	Time 	10
	
	Event
		EName 	LaserGlowOuter
		Type	local
		At	WepNode
		Part	:MaltaLaserGlow1.part
		Lifespan 45

	End

		
End

Condition
	On 	Time
	Time 	20
	
	Event
		EName 	LaserGlowInner
		Type	local
		At	WepNode
		Part	:MaltaLaserGlowInner2.part
		Lifespan 33

	End

		
End

Condition
	On 	Time
	Time 	30
	
	Event
		EName 	LaserGlowInner
		Type	local
		At	WepNode
		Part	:MaltaLaserGlowStar1.part
		Lifespan 31

	End

		
End

Condition
	On 	Time
	Time 	35
	
	Event
		EName 	LaserGlowStar
		Type	local
		At	WepNode
		Part	:MaltaLaserGlowInner.part
		Pmagnet WepNode
		Lifespan 15

	End

		
End

Condition
	On 	Time
	Time 	60
	
	Event
		EName 	LaserGlowStar
		Type	Start
		At	WepNode
		Part	:MaltaLaserGlowPop.part
		Part	:MaltaLaserBeam04.part
		Part	:MaltaLaserBeam03.part
		Part	:MaltaLaserBeam01.part
		Part	:MaltaLaserBeam02.part
		POther Target
		Lifespan 2

	End

	
	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	:LaserLights.bhvr
		LifeSpan 5
	End

		
End

Condition
	On 	Time
	Time 	61
	
	Event
		EName 	LaserHit
		Type	Start
		At	Target
		#Part	:MaltaLaserHitGlowInnerSmall.part
		Part	:MaltaLaserHitStar1Small.part
		Part	:MaltaLaserHitPopSmall.part
		Lifespan 2

	End

		
End


End			