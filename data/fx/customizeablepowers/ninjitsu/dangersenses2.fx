#########################################################
##	template

FxInfo


#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		Sound Fast 80 80 .8
	End
End

#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Cycle
	Time	30
	Chance	.7
	

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Head
		part1	CustomizeablePowers\Ninjitsu\HeadCircles01.part
		part1	CustomizeablePowers\Ninjitsu\HeadCirclesDark.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 30
	
	End


End

Condition
	On	Time
	Time	60
	

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Head
		part1	CustomizeablePowers\Ninjitsu\HeadSpikes01.part
		part1	CustomizeablePowers\Ninjitsu\HeadSpikesDark01.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 30
	
	End


End

Condition
	On	Cycle
	Time	60
	Chance	.7
	

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Head
		part1	CustomizeablePowers\Ninjitsu\HeadSpikes02.part
		part1	CustomizeablePowers\Ninjitsu\HeadSpikesDark02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 25
	
	End


End


Condition
	On	Time
	Time	0
	
	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	CustomizeablePowers\Ninjitsu\BodyGlowLines01.part
		part1	CustomizeablePowers\Ninjitsu\BodyGlows01.part
		Part	CustomizeablePowers\Ninjitsu\BodyWavesDark.part
		POther Neck
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Head
		#part1	:BodyGlowLines01.part
		part1	CustomizeablePowers\Ninjitsu\BodyGlows01.part
		Part	CustomizeablePowers\Ninjitsu\BodyWavesDark.part
		POther Chest
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End
	

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	CustomizeablePowers\Ninjitsu\BodyGlowLines01.part
		part1	CustomizeablePowers\Ninjitsu\BodyGlows01.part
		Part	CustomizeablePowers\Ninjitsu\BodyWavesDark.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	CustomizeablePowers\Ninjitsu\BodyStreaks01.part
		part1	CustomizeablePowers\Ninjitsu\BodyGlowLines01.part
		part1	CustomizeablePowers\Ninjitsu\BodyGlows01.part
		Part	CustomizeablePowers\Ninjitsu\BodyWavesDark.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End


	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	CustomizeablePowers\Ninjitsu\BodyGlowLines01.part
		part1	CustomizeablePowers\Ninjitsu\BodyGlows01.part
		Part	CustomizeablePowers\Ninjitsu\BodyWavesDark.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	CustomizeablePowers\Ninjitsu\BodyStreaks01.part
		part1	CustomizeablePowers\Ninjitsu\BodyGlowLines01.part
		part1	CustomizeablePowers\Ninjitsu\BodyGlows01.part
		Part	CustomizeablePowers\Ninjitsu\BodyWavesDark.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End


	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part1	CustomizeablePowers\Ninjitsu\BodyGlowLines01.part
		part1	CustomizeablePowers\Ninjitsu\BodyGlows01.part
		Part	CustomizeablePowers\Ninjitsu\BodyWavesDark.part
		POther Ulegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part1	CustomizeablePowers\Ninjitsu\BodyStreaks01.part
		part1	CustomizeablePowers\Ninjitsu\BodyGlowLines01.part
		part1	CustomizeablePowers\Ninjitsu\BodyGlows01.part
		Part	CustomizeablePowers\Ninjitsu\BodyWavesDark.part
		POther Llegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part1	CustomizeablePowers\Ninjitsu\BodyGlowLines01.part
		part1	CustomizeablePowers\Ninjitsu\BodyGlows01.part
		Part	CustomizeablePowers\Ninjitsu\BodyWavesDark.part
		POther UlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	CustomizeablePowers\Ninjitsu\BodyStreaks01.part
		part	CustomizeablePowers\Ninjitsu\BodyGlowLines01.part
		part	CustomizeablePowers\Ninjitsu\BodyGlows01.part
		Part	CustomizeablePowers\Ninjitsu\BodyWavesDark.part
		POther LlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End


End

End		