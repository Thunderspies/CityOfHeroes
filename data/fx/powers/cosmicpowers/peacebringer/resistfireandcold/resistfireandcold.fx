#########################################################
##	FireAura
FxInfo

##################################
#################################################################
Condition
	On 	Time
	Time 	0

	Event
		EName 	Primea
		Type	Local 
		At	Eyes
		BHVR	Behaviors/OffsetEyes.bhvr
		Geom	CircleOfThorns
		sound	peaceeyes1 60 50 .77
		#LifeSpan	30
	End

	Event
		Type	Local 
		At	Primea
		 AltPiv 1
		BHVR	Behaviors/GenericparticleFade.bhvr
		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\CircleOfThornsEyes_21Green.part
		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\EyesMistLeftBLUE.part
		Sound regen4 60 60 .5
		#LifeSpan	30
	End
	
	Event
		Type	Local 
		At	Primea
		 AltPiv 2
		BHVR	Behaviors/GenericparticleFade.bhvr
		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\CircleOfThornsEyes_21Green.part
		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\EyesMistRightBLUE.part
		Sound glow6_loop 50 50 .22
		#LifeSpan	30
	End

End
######################################################################
Condition
	
	Event
		Type	local
		At	root
		Bhvr 	:Resistfireandcold.bhvr
		
		Geom	CosmicPeaceBringerShield
		LifeSpan	10
				
	End
	
	Event
		Type	local
		At	root
		Bhvr 	:RingsUp.bhvr
		#Part	:RingCoreenergy.part
		Part	:RingTendril01.part
		LifeSpan	35		
	End
	
	Event
		Type	local
		At	root
		Bhvr 	:RingsDown.bhvr
		#Part	:RingCoreenergy.part		
		Part	:RingTendril01.part
		LifeSpan	35		
	End

End

Condition
	On	Time
	Time	27

#	Event
#		Type	local
#		At	root
#		Bhvr 	:TopOffset.bhvr
#		Part	:RingTendrilBaseDown.part
#
#	End
	
	Event
		Type	local
		At	root
		Bhvr 	:BottemOffset.bhvr
		Part	:RingTendrilBase.part

	End

End

Condition
	On 	cycle
	Time 	25

	Event
		Type	local
		At	root
		Bhvr 	:Resistfireandcold.bhvr
		
		Geom	CosmicPeaceBringerShield
		LifeSpan	10
				
	End

End

Condition
	On 	cycle
	Time 	30

	Event
		Type	local
		At	root
		Bhvr 	:RingsUp.bhvr
		#Part	:RingCoreenergy.part
		Part	:RingTendril01.part
		LifeSpan	20		
	End

End

Condition
	On 	cycle
	Time 	30

	Event
		Type	local
		At	root
		Bhvr 	:RingsDown.bhvr
		#Part	:RingCoreenergy.part		
		Part	:RingTendril01.part
		LifeSpan	20		
	End

End

End				