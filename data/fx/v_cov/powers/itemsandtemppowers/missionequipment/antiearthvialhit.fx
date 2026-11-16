#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	InpName	Root
End

LifeSpan	120

######################################################################


Condition
	On	Time
	Time	0

	Event
		Type	Local	
		At	Origin
		Sound vialgasexplo 80 80 .9
	End
End

#Condition
#	On	Time
#	Time	5
#
#	Event
#		Type	Local	
#		At	Origin
#		Sound	miasma_loop 80 50 .25
#	End
#End



Condition
	On 	Time
	Time 	0

	Event
		EName 	StoneDustRingHit
		Type	SetLight
		Bhvr	:Hitlightpulse.bhvr	
		LifeSpan	45
	End

	Event
		EName 	StoneDustRingHit
		Type	local
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr			
		Part	:AntiEarthVialGlow.part
		Part	:AntiEarthVialShine.part
		Part	:AntiEarthVialTrail.part
		LifeSpan	15
	End

	Event
		EName 	StoneDustRingHit
		Type	local
		At	Head
		Bhvr	Behaviors/GenericParticleFade.bhvr			
		Part	:AntiEarthVialGlow.part
		Part	:AntiEarthVialShine.part
		Part	:AntiEarthVialTrail.part
		LifeSpan	15
	End

	Event
		EName 	StoneDustRingHit
		Type	Local
		At	Hips
		Bhvr	Behaviors/GenericParticleFade.bhvr			
		Part	:AntiEarthVialGlow.part
		Part	:AntiEarthVialShine.part
		Part	:AntiEarthVialTrail.part
		LifeSpan	15
	End

	Event
		EName 	StoneDustRingHit
		Type	local
		At	UarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr			
		Part	:AntiEarthVialGlow.part
		Part	:AntiEarthVialShine.part
		Part	:AntiEarthVialTrail.part
		LifeSpan	15
	End

	Event
		EName 	StoneDustRingHit
		Type	Local
		At	UarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr			
		Part	:AntiEarthVialGlow.part
		Part	:AntiEarthVialShine.part
		Part	:AntiEarthVialTrail.part
		LifeSpan	15
	End


	Event
		EName 	StoneDustRingHit
		Type	local
		At	LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr			
		Part	:AntiEarthVialGlow.part
		Part	:AntiEarthVialShine.part
		Part	:AntiEarthVialTrail.part
		LifeSpan	15
	End

	Event
		EName 	StoneDustRingHit
		Type	Local
		At	LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr			
		Part	:AntiEarthVialGlow.part
		Part	:AntiEarthVialShine.part
		Part	:AntiEarthVialTrail.part
		LifeSpan	15
	End

	Event
		EName 	StoneDustRingHit
		Type	local
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr	
		Part	:AntiEarthVialTrail.part
		LifeSpan	120
	End

	Event
		EName 	StoneDustRingHit
		Type	local
		At	Head
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:AntiEarthVialTrail.part
		LifeSpan	120
	End

	Event
		EName 	StoneDustRingHit
		Type	Local
		At	Hips
		Bhvr	Behaviors/GenericParticleFade.bhvr	
		Part	:AntiEarthVialTrail.part
		LifeSpan	120
	End


	Event
		EName 	StoneDustRingHit
		Type	local
		At	LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr	
		Part	:AntiEarthVialTrail.part
		LifeSpan	120
	End

	Event
		EName 	StoneDustRingHit
		Type	Local
		At	LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:AntiEarthVialTrail.part
		LifeSpan	120
	End

End


End
##################################

