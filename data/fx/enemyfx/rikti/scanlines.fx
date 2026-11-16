#########################################################
##	RadiationAura
FxInfo

LifeSpan	60
#############################################################

Condition

	Event
		EName	Node1
		Type	start
		At	root

		BhvrOverride
			
			InitialVelocity	  0.0 0.5 0.0
			PositionOffset    5 0 0
#			PyrRotate	  90 0 0
		End

		Part	:LineUp.part
		
		Magnet	Node2
		
		LifeSpan	20

	End
	
	Event
		EName	Node2
		Type	start
		At	root

		BhvrOverride
			InitialVelocity	  0.0 0.5 0.0
			PositionOffset	-5 0 0 
#			PyrRotate	90 0 0
		End

		Part	:LineUp.part

		Magnet	Node1
		
		LifeSpan	20
	End

################################################################################

End


End				