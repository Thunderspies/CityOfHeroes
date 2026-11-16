#########################################################
##	Fireweapon
########################################################
FxInfo

##########################################
##  AUDIO
##########################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	Origin
		#Sound	rumble2_loop 80 60 .5

	End
End

#########################################################
########################################################

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		EName 	Primea
#		Type	Local 
#		At	T_Eyes
#		BHVR	Behaviors/OffsetEyes.bhvr
#		Geom	CircleOfThorns
#		LifeSpan	230
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
#		LifeSpan	230
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
#		LifeSpan	230
#	End
#
#End
######################################################################
Condition
	On 	Time
	Time 	0

	Event
		EName 	Hurricane
		Type	local 
		At	root
		BHVR	Behaviors/PortalPivot.bhvr
		#Part	:BlackHole.part
		#Part	:BlackHolePurple.part
		#Part	:BlackHoleBlue.part
		#Part	:BlackHoleOutterRings.part
		Part	:BlackHoleOutterRingsIn.part
		#Part	:PortalThickness.part
		Part	:InwardPortalMatter.part
		#Part	:BlackHoleCore.part

	End


End

End
##################################

