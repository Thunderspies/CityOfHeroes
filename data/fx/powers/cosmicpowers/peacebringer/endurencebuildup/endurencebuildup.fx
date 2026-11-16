#############################################################
## FX System Name
#############################################################

FxInfo

Lifespan 75

#############################################################

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		EName 	Primea
#		Type	Local 
#		At	Eyes
#		BHVR	Behaviors/OffsetEyes.bhvr
#		Geom	CircleOfThorns
#		sound	peaceeyes1 60 50 .7
#		LifeSpan	30
#	End
#
#	Event
#		Type	Local 
#		At	Primea
#		AltPiv	1
#		BHVR	Behaviors/GenericparticleFade.bhvr
#		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\CircleOfThornsEyes_21Green.part
#		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\EyesMistLeftBLUE.part
#	
#		LifeSpan	30
#	End
#	
#	Event
#		Type	Local 
#		At	Primea
#		AltPiv	2
#		BHVR	Behaviors/GenericparticleFade.bhvr
#		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\CircleOfThornsEyes_21Green.part
#		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\EyesMistRightBLUE.part
#		LifeSpan	30
#	End
#End

######################################################################

Condition
	On	time
	Time	0

	Event

		Type	Local
		At	Chest
		Sound aim3 60 60 .6
	End
End

Condition
	On	Time 
	Time	0
	
	Event
		Type 	local
		At	Chest
		Part	:EndurenceTendrils.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan	30
	End

	Event
		EName	Pow2
		Type 	local
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part1	:BuildUpGlow.part
		Part2	:BuildUpSpecks2.part
		Lifespan	15
	End
End

Condition
	On	Time 
	Time	10
	
	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:BuildUpSpecks.part
		Part2	:BuildUpRing01.part
		Part3	:BuildUpRing01a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End
End

Condition
	On	Time 
	Time	20
	
	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:BuildUpSpecks.part
		Part3	:BuildUpRing02.part
		Part4	:BuildUpRing02a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

End

Condition
	On	Time 
	Time	30
	
	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:BuildUpSpecks2.part
		Part2	:BuildUpRingSlow.part
		Part3	:BuildUpRing03.part
		Part4	:BuildUpRing03a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End
End

#############################################################	

End