#########################################################
##	FireAura
FxInfo

##################################
########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Primea
		Type	Local 
		At	Eyes
		BHVR	Behaviors/OffsetEyes.bhvr
		Geom	CircleOfThorns
		Sound Warshadeeyes3 60 60 .77
		#LifeSpan	30
	End

	Event
		Type	Local 
		At	Primea
		 AltPiv 1
		 BHVR	Behaviors/GenericParticleFade.bhvr
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyesMistLeftBLK.part
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyes_21Yellow.part
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyes_21Green.part
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\Eyes_BLUE.part
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\Eyes_PURPLE.part
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyes_BLACK.part
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyesMistLeft.part
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyesMistLeftBLUE.part
		#LifeSpan	30
		
	End
	
	Event
		Type	Local 
		At	Primea
		 AltPiv 2
		 BHVR	Behaviors/GenericParticleFade.bhvr
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyesMistRightBLK.part
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyes_21Yellow.part
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyes_21Green.part
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\Eyes_BLUE.part
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\Eyes_PURPLE.part
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyes_BLACK.part
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyesMistRight.part
		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyesMistRightBLUE.part
		#LifeSpan	30
	End

End
######################################################################
Condition
	On 	Time
	Time 	0
	Event

		Type	Local
		At	Origin
		Sound WarshadeBuild 70 70 .76
	End
	
End

Condition
	On 	Time
	Time 	12
	Event

		Type	Local
		At	Origin
		Sound hydrapod2_loop 60 60 .2
	End
	
End


Condition
	
	Event
		Type	local
		At	root
		Bhvr 	:Resistfireandcold.bhvr
		
		Geom	CosmicNictusShield
		LifeSpan	45
				
	End
	
	Event
		Type	local
		At	root
		Bhvr 	:RingsUp.bhvr
		Part	:RingTendril01.part
		Part	:RingTendrilPurple.part
		Part	:RingTendrilBLK.part
		LifeSpan	20		
	End
	
	Event
		Type	local
		At	root
		Bhvr 	:RingsDown.bhvr
		Part	:RingTendril01.part
		Part	:RingTendrilPurple.part
		Part	:RingTendrilBLK.part
		LifeSpan	20		
	End

End

Condition
	On 	cycle
	Time 	25

	Event
		Type	local
		At	root
		Bhvr 	:Resistfireandcold.bhvr
		
		Geom	CosmicNictusShield
		LifeSpan	45
				
	End

End

Condition
	On 	cycle
	Time 	30

	Event
		Type	local
		At	root
		Bhvr 	:RingsUp.bhvr
		Part	:RingTendril01.part
		Part	:RingTendrilPurple.part
		Part	:RingTendrilBLK.part
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
		Part	:RingTendril01.part
		Part	:RingTendrilPurple.part
		Part	:RingTendrilBLK.part
		LifeSpan	20		
	End

End

End				