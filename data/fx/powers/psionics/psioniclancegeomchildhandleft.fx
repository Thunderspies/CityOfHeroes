#########################################################
##	Fire_Ball
##
FxInfo

#####################################################################################
lifeSpan	60
##############################################################################

#Condition
#		
#	Event
#		Type	Local
#		At	HandR
#		
#		BhvrOverride
#
#			Scale	2 1.5 1.5
##			Alpha	155
#		End
#
#		Geom	PsionicSpikeRight
#
#	End
#		
#	Event
#		Type	Local
#		At	HandL
#		
#		BhvrOverride
#
#			Scale	2 1.5 1.5
##			Alpha	155
#		End
#
#		Geom	PsionicSpikeLeft
#
#	End
#	
############################ ARM ############################

Condition

	Event
		Type	Local
		At	HandL
		
		Bhvr	:SpikescaleUp.bhvr

		Geom	PsionicSpikeLeft

		LifeSpan	30
	End

End

Condition
	On	Time
	Time	30

	Event
		Type	Local
		At	HandL
		
		Bhvr	:SpikescaleDown.bhvr

		Geom	PsionicSpikeLeft

		LifeSpan	30
	End

End

############################ ARMS ############################

#Condition
#
#	Event
#		Type	Local
#		At	Back
#		
#		Bhvr	:BackTupescaleUP.bhvr
#		Geom	PsionicSpikeBack
#
#		LifeSpan	29
#
#	End
#
#End
#
#Condition
#	On	Time
#	Time	30
#
#	Event
#		Type	Local
#		At	Back
#		
#		Bhvr	:BackTupescaleDown.bhvr
#		Geom	PsionicSpikeBack
#
#		LifeSpan	29
#
#	End
#
#End

########################### Back ############################

#Condition
#
#	Event
#		Type	Local
#		At	SpadL
#		
#		BhvrOverride
#
#			PYRrotate	30 0 0
#			PositionOffset		-.2 .3 0.05
#			#Scale	4 4 4	
#		End
#
#		Bhvr	:TupescaleUP.bhvr
#		
#		Geom	PsionicSpikeLeftShoulder
#
#		LifeSpan	28
#
#	End
#
#End
#
#Condition
#	On	Time
#	Time	30
#
#	Event
#		Type	Local
#		At	SpadL
#		
#		BhvrOverride
#
#			PYRrotate	30 0 0
#			PositionOffset		-.2 .3 0.05
#			#Scale	4 4 4	
#		End
#
#		Bhvr	:TupescaleDown.bhvr
#		
#		Geom	PsionicSpikeLeftShoulder
#
#		LifeSpan	28
#
#	End
#
#End
#

End