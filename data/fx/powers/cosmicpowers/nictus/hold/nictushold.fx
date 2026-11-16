#########################################################
##	chill touch hit
FxInfo

############################################################
########################################################

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
#		LifeSpan	30
#	End
#
#	Event
#		Type	Local 
#		At	Primea
#		 AltPiv 1
#		 BHVR	Behaviors/GenericParticleFade.bhvr
#		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyesMistLeftBLK.part
#		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyes_21Yellow.part
#		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyes_21Green.part
#		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\Eyes_BLUE.part
#		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\Eyes_PURPLE.part
#		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyes_BLACK.part
#		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyesMistLeft.part
#		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyesMistLeftBLUE.part
#		LifeSpan	30
#		
#	End
#	
#	Event
#		Type	Local 
#		At	Primea
#		 AltPiv 2
#		 BHVR	Behaviors/GenericParticleFade.bhvr
#		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyesMistRightBLK.part
#		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyes_21Yellow.part
#		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyes_21Green.part
#		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\Eyes_BLUE.part
#		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\Eyes_PURPLE.part
#		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyes_BLACK.part
#		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyesMistRight.part
#		Part	POWERS\CosmicPowers\Nictus\ActivationEyes\CircleOfThornsEyesMistRightBLUE.part
#		LifeSpan	30
#	End
#
#End
######################################################################
Condition

	Event
		EName 	Orb2
		Type	Local 
		At	Root
		Part	:MistHoldPurpleBaseLight.part
		Part	:MistHoldBlueBaseLight.part
		Part	:MistHoldBlackBaseLight.part
		part	:HoldFireFlies.part
		part	:MistHold.part
		part	:MistHoldBlue.part
		part	:MistHoldPurple.part
		part	:MistHoldBlack.part
		Sound Warshadehit1 80 80 .6
	End

	Event
		Type	Local 
		At	chest
		
		part	:TendrilHoldRing.part
		part	:PurpleTendrilHoldRing.part
		part	:BlueTendrilHoldRing.part
		Sound WarHold2_loop 60 60 .66
	End

	Event
		Ename	GravitationalForce
		Type	Local
		At	Chest
		
		BhvrOverride
			PhysForceType		In
			PhysForceRadius		15
			PhysForcePower		60
			PhysForcePowerJitter	15
		End

	End
End

End			