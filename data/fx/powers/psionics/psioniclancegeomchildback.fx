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
############################ARMS############################
#		
#	Event
#		Type	Local
#		At	LarmL
#		
#		BhvrOverride
#
#			Scale	2 1.5 1.5
##			Alpha	155
#		End
#
#		Geom	PsionicSpikeLarmRight
#
#	End
#		
############################ ARMS ############################

Condition

	Event
		Type	Local
		At	Back
		
		Bhvr	:BackTupescaleUP.bhvr
		Geom	PsionicSpikeBack

		LifeSpan	28

	End

End

Condition
	On	Time
	Time	30

	Event
		Type	Local
		At	Back
		
		Bhvr	:BackTupescaleDown.bhvr
		Geom	PsionicSpikeBack

		LifeSpan	28

	End

End

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
#		LifeSpan	30
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
#		LifeSpan	30
#
#	End
#
#End
#

End