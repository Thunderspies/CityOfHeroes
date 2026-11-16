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

########################################################
###################### Audio ###########################
########################################################

Condition
	On	Time
	Time	10

	Event	
		Type	Local
		At	Origin
		Sound servox1 140 140 .65
	End

End


Condition
	On	Time
	Time	17

	Event	
		Type	Local
		At	Origin
		Sound titanlaser1 140 140 1.0
	End

End






Condition
	On	Time
	Time	34

	Event	
		Type	Local
		At	Origin
		Sound laserfire1 140 140 .88
	End

End


Condition
	On	Time
	Time	46

	Event	
		Type	Local
		At	Origin
		Sound laserblast2 140 140 .75
	End

End




########################################################
###################### Right Hand ######################
########################################################

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	WepNodeR
		Type	local
		At	WepR
		Bhvr	:QuadLaserOffsetRt.bhvr
		Lifespan 200

	End

		
End

Condition
	On 	Time
	Time 	20
	
	Event
		EName 	LaserGlowOuter
		Type	local
		At	WepNodeR
		Part	:MaltaLaserGlow1.part
		Lifespan 45

	End

		
End

Condition
	On 	Time
	Time 	30
	
	Event
		EName 	LaserGlowInner
		Type	local
		At	WepNodeR
		Part	:MaltaLaserGlowInner2.part
		Lifespan 33

	End

		
End

Condition
	On 	Time
	Time 	38
	
	Event
		EName 	LaserGlowStar
		Type	Start
		At	WepNodeR
		Part	:MaltaLaserGlowPopSmall.part
		Part	:MaltaLaserBeam02small.part
		Part	:MaltaLaserBeam03small.part
		Part	:MaltaLaserBeam04small.part
		POther Target
		Lifespan 1

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
	Time 	39
	
	Event
		EName 	LaserHit
		Type	Start
		At	Target
		Part	:MaltaQLaserHitGlowInner.part
		Part	:MaltaQLaserHitStar2.part
		Part	:MaltaQLaserHitSparks.part
		Lifespan 1

	End

		
End

#######################################################

Condition
	On 	Time
	Time 	48
	
	Event
		EName 	LaserGlowStar
		Type	Start
		At	WepNodeR
		Part	:MaltaLaserGlowPopSmall.part
		Part	:MaltaLaserBeam02small.part
		Part	:MaltaLaserBeam03small.part
		Part	:MaltaLaserBeam04small.part
		POther Target
		Lifespan 1

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
	Time 	49
	
	Event
		EName 	LaserHit
		Type	Start
		At	Target
		Part	:MaltaQLaserHitGlowInner.part
		Part	:MaltaQLaserHitStar2.part
		Part	:MaltaQLaserHitSparks.part
		Lifespan 1

	End

		
End

########################################################

Condition
	On 	Time
	Time 	58
	
	Event
		EName 	LaserGlowStar
		Type	Start
		At	WepNodeR
		Part	:MaltaLaserGlowPopSmall.part
		Part	:MaltaLaserBeam02small.part
		Part	:MaltaLaserBeam03small.part
		Part	:MaltaLaserBeam04small.part
		POther Target
		Lifespan 1

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
	Time 	59
	
	Event
		EName 	LaserHit
		Type	Start
		At	Target
		Part	:MaltaQLaserHitGlowInner.part
		Part	:MaltaQLaserHitStar2.part
		Part	:MaltaQLaserHitSparks.part
		Lifespan 1

	End

		
End

########################################################
###################### Left Hand #######################
########################################################

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	WepNodeL
		Type	local
		At	WepL
		Bhvr	:QuadLaserOffsetLt.bhvr
		Lifespan 200

	End

		
End

Condition
	On 	Time
	Time 	23
	
	Event
		EName 	LaserGlowOuter
		Type	local
		At	WepNodeL
		Part	:MaltaLaserGlow1.part
		Lifespan 45

	End

		
End

Condition
	On 	Time
	Time 	33
	
	Event
		EName 	LaserGlowInner
		Type	local
		At	WepNodeL
		Part	:MaltaLaserGlowInner2.part
		Lifespan 33

	End

		
End

Condition
	On 	Time
	Time 	41
	
	Event
		EName 	LaserGlowStar
		Type	Start
		At	WepNodeL
		Part	:MaltaLaserGlowPopSmall.part
		Part	:MaltaLaserBeam02small.part
		Part	:MaltaLaserBeam03small.part
		Part	:MaltaLaserBeam04small.part
		POther Target
		Lifespan 1

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
	Time 	42
	
	Event
		EName 	LaserHit
		Type	Start
		At	Target
		Part	:MaltaQLaserHitGlowInner.part
		Part	:MaltaQLaserHitStar2.part
		Part	:MaltaQLaserHitSparks.part
		Lifespan 1

	End

		
End

#######################################################

Condition
	On 	Time
	Time 	51
	
	Event
		EName 	LaserGlowStar
		Type	Start
		At	WepNodeL
		Part	:MaltaLaserGlowPopSmall.part
		Part	:MaltaLaserBeam02small.part
		Part	:MaltaLaserBeam03small.part
		Part	:MaltaLaserBeam04small.part
		POther Target
		Lifespan 1

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
	Time 	52
	
	Event
		EName 	LaserHit
		Type	Start
		At	Target
		Part	:MaltaQLaserHitGlowInner.part
		Part	:MaltaQLaserHitStar2.part
		Part	:MaltaQLaserHitSparks.part
		Lifespan 1

	End

		
End

########################################################

Condition
	On 	Time
	Time 	61
	
	Event
		EName 	LaserGlowStar
		Type	Start
		At	WepNodeL
		Part	:MaltaLaserGlowPopSmall.part
		Part	:MaltaLaserBeam02small.part
		Part	:MaltaLaserBeam03small.part
		Part	:MaltaLaserBeam04small.part
		POther Target
		Lifespan 1

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
	Time 	62
	
	Event
		EName 	LaserHit
		Type	Start
		At	Target
		Part	:MaltaQLaserHitGlowInner.part
		Part	:MaltaQLaserHitStar2.part
		Part	:MaltaQLaserHitSparks.part
		Lifespan 1

	End

		
End

########################################################
###################### Low Right #######################
########################################################

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	LowWepNodeR
		Type	local
		At	Chest
		Bhvr	:QuadLaserOffsetLowRt.bhvr
		Lifespan 200

	End

		
End

Condition
	On 	Time
	Time 	17
	
	Event
		EName 	LaserGlowOuter
		Type	local
		At	LowWepNodeR
		Part	:MaltaLaserGlow1.part
		Lifespan 45

	End

		
End

Condition
	On 	Time
	Time 	27
	
	Event
		EName 	LaserGlowInner
		Type	local
		At	LowWepNodeR
		Part	:MaltaLaserGlowInner2.part
		Lifespan 33

	End

		
End

Condition
	On 	Time
	Time 	35
	
	Event
		EName 	LaserGlowStar
		Type	Start
		At	LowWepNodeR
		Part	:MaltaLaserGlowPopSmall.part
		Part	:MaltaLaserBeam02small.part
		Part	:MaltaLaserBeam03small.part
		Part	:MaltaLaserBeam04small.part
		POther Target
		Lifespan 1

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
	Time 	36
	
	Event
		EName 	LaserHit
		Type	Start
		At	Target
		Part	:MaltaQLaserHitGlowInner.part
		Part	:MaltaQLaserHitStar2.part
		Part	:MaltaQLaserHitSparks.part
		Lifespan 1

	End

		
End

#######################################################

Condition
	On 	Time
	Time 	45
	
	Event
		EName 	LaserGlowStar
		Type	Start
		At	LowWepNodeR
		Part	:MaltaLaserGlowPopSmall.part
		Part	:MaltaLaserBeam02small.part
		Part	:MaltaLaserBeam03small.part
		Part	:MaltaLaserBeam04small.part
		POther Target
		Lifespan 1

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
	Time 	46
	
	Event
		EName 	LaserHit
		Type	Start
		At	Target
		Part	:MaltaQLaserHitGlowInner.part
		Part	:MaltaQLaserHitStar2.part
		Part	:MaltaQLaserHitSparks.part
		Lifespan 1

	End

		
End

########################################################

Condition
	On 	Time
	Time 	55
	
	Event
		EName 	LaserGlowStar
		Type	Start
		At	LowWepNodeR
		Part	:MaltaLaserGlowPopSmall.part
		Part	:MaltaLaserBeam02small.part
		Part	:MaltaLaserBeam03small.part
		Part	:MaltaLaserBeam04small.part
		POther Target
		Lifespan 1

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
	Time 	56
	
	Event
		EName 	LaserHit
		Type	Start
		At	Target
		Part	:MaltaQLaserHitGlowInner.part
		Part	:MaltaQLaserHitStar2.part
		Part	:MaltaQLaserHitSparks.part
		Lifespan 1

	End

		
End


########################################################
###################### Low Left ########################
########################################################

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	LowWepNodeL
		Type	local
		At	Chest
		Bhvr	:QuadLaserOffsetLowLt.bhvr
		Lifespan 200

	End

		
End

Condition
	On 	Time
	Time 	26
	
	Event
		EName 	LaserGlowOuter
		Type	local
		At	LowWepNodeL
		Part	:MaltaLaserGlow1.part
		Lifespan 45

	End

		
End

Condition
	On 	Time
	Time 	36
	
	Event
		EName 	LaserGlowInner
		Type	local
		At	LowWepNodeL
		Part	:MaltaLaserGlowInner2.part
		Lifespan 33

	End

		
End

Condition
	On 	Time
	Time 	44
	
	Event
		EName 	LaserGlowStar
		Type	Start
		At	LowWepNodeL
		Part	:MaltaLaserGlowPopSmall.part
		Part	:MaltaLaserBeam02small.part
		Part	:MaltaLaserBeam03small.part
		Part	:MaltaLaserBeam04small.part
		POther Target
		Lifespan 1

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
	Time 	45
	
	Event
		EName 	LaserHit
		Type	Start
		At	Target
		Part	:MaltaQLaserHitGlowInner.part
		Part	:MaltaQLaserHitStar2.part
		Part	:MaltaQLaserHitSparks.part
		Lifespan 1

	End

		
End

#######################################################

Condition
	On 	Time
	Time 	54
	
	Event
		EName 	LaserGlowStar
		Type	Start
		At	LowWepNodeL
		Part	:MaltaLaserGlowPopSmall.part
		Part	:MaltaLaserBeam02small.part
		Part	:MaltaLaserBeam03small.part
		Part	:MaltaLaserBeam04small.part
		POther Target
		Lifespan 1

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
	Time 	45
	
	Event
		EName 	LaserHit
		Type	Start
		At	Target
		Part	:MaltaQLaserHitGlowInner.part
		Part	:MaltaQLaserHitStar2.part
		Part	:MaltaQLaserHitSparks.part
		Lifespan 1

	End

		
End

########################################################

Condition
	On 	Time
	Time 	54
	
	Event
		EName 	LaserGlowStar
		Type	Start
		At	LowWepNodeL
		Part	:MaltaLaserGlowPopSmall.part
		Part	:MaltaLaserBeam02small.part
		Part	:MaltaLaserBeam03small.part
		Part	:MaltaLaserBeam04small.part
		POther Target
		Lifespan 1

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
	Time 	65
	
	Event
		EName 	LaserHit
		Type	Start
		At	Target
		Part	:MaltaQLaserHitGlowInner.part
		Part	:MaltaQLaserHitStar2.part
		Part	:MaltaQLaserHitSparks.part
		Lifespan 5

	End

		
End



End			