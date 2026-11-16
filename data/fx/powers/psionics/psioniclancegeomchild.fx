#########################################################
##	Fire_Ball
##
FxInfo

#####################################################################################
lifeSpan	160
##############################################################################

########################### Back ############################

Condition

	Event
		Type	Local
		At	SpadL
		
		BhvrOverride

			PYRrotate	30 0 0
			PositionOffset		-.2 .3 0.05
			#Scale	4 4 4	
		End

		Bhvr	:ShoulderTupescaleUP.bhvr
		
		Geom	PsionicSpikeLeftShoulder

		LifeSpan	35

	End

End

Condition
	On	Time
	Time	40

	Event
		Type	Local
		At	SpadL
		
		BhvrOverride

			PYRrotate	30 0 0
			PositionOffset		-.2 .3 0.05
			#Scale	4 4 4	
		End

		Bhvr	:ShoulderTupescaleDown.bhvr
		
		Geom	PsionicSpikeLeftShoulder

		LifeSpan	50

	End

End


End