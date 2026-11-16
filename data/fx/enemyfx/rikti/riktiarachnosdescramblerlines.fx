#########################################################
##	RadiationAura
FxInfo

#LifeSpan	60

###########################################################################################
##
Condition

	Event
		Ename	Node1
		Type	local
		At	Root
		
		BhvrOverride
			InitialVelocity		0 .3 0
			PositionOffset		-10 0 0
		End

		Part	:ElectricSpark.part
		
		POther	Node2
		LifeSpan	100
	End

	Event
		Ename	Node2
		Type	local
		At	Root
		
		BhvrOverride			
			InitialVelocity		0 .3 0
			PositionOffset		10 0 0
		End

		Part	:ElectricSpark.part
		
		POther	Node1
		LifeSpan	100
	End

End

Condition
	On	Time
	Time	35
			
	Event
		Ename	Node1
		Type	Destroy
	End
		
	Event
		Ename	Node2
		Type	Destroy
	End

End

End				