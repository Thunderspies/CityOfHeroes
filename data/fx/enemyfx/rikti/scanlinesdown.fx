#########################################################
##	RadiationAura
FxInfo

LifeSpan	160
#############################################################

Condition

	Event
		EName	Node1
		Type	local
		At	Origin

		BhvrOverride
			
			InitialVelocity	  0.0 -0.5 0.0
			PositionOffset    5 11 0
#			PyrRotate	  90 0 0
		End

		Part	:LineDown.part
		
		Magnet	Node2
		
		LifeSpan	80

	End
	
	Event
		EName	Node2
		Type	local
		At	Origin

		BhvrOverride
			InitialVelocity	  0.0 -0.5 0.0
			PositionOffset	-5 11 0 
#			PyrRotate	90 0 0
		End

		Part	:LineUp.part

		Magnet	Node1
		
		LifeSpan	80
	End

################################################################################

End


End				