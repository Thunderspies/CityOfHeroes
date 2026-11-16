#########################################################
##	RadiationAura
FxInfo

LifeSpan	60
#############################################################

Condition
###########################################################################################

	Event
		EName	Node1
		Type	start
		At	origin

		BhvrOverride
			
			InitialVelocity	  0.0 0.25 0.0
			PositionOffset    .5 0 -1.128
			PyrRotate	  90 0 0
		End
		
		Magnet	Node2
		
		LifeSpan	33

	End
	
	Event
		EName	Node2
		Type	start
		At	origin

		BhvrOverride
			InitialVelocity	  0.0 0.25 0.0
			PositionOffset	.5 0 1.125 
			PyrRotate	90 0 0
		End

		Magnet	Node1
		
		LifeSpan	33
	End
	
####################################################################################

	Event
		EName	Node1
		Type	start
		At	origin

		BhvrOverride
			
			InitialVelocity	  0.0 0.25 0.0
			PositionOffset   -.5 0 -.51
			PyrRotate	  90 0 0
		End

		Part	:LineUp2.part
		
		Magnet	Node2
		
		LifeSpan	33

	End
	
	Event
		EName	Node2
		Type	start
		At	origin

		BhvrOverride
			InitialVelocity	  0.0 0.25 0.0
			PositionOffset	-.5 0 -.51 
			PyrRotate	90 0 0
		End

		Part	:LineUp2.part

		Magnet	Node1
		
		LifeSpan	33
	End


####################################################################################

	Event
		EName	Node1
		Type	start
		At	origin

		BhvrOverride
			
			InitialVelocity	  0.0 0.25 0.0
			PositionOffset   -.5 0 -.51
			PyrRotate	  90 0 0
		End

		Part	:LineUp3.part
		
		Magnet	Node2
		
		LifeSpan	33

	End
	
	Event
		EName	Node2
		Type	start
		At	origin

		BhvrOverride
			InitialVelocity	  0.0 0.25 0.0
			PositionOffset	-.5 0 -.51 
			PyrRotate	90 0 0
		End

		Part	:LineUp3.part

		Magnet	Node1
		
		LifeSpan	33
	End

End

End				