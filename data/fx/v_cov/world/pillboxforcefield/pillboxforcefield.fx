#########################################################
##	forceBubble
########################################################
FxInfo





Condition
	On	Time
	Time	0

	Event	
		Type	Local
		At	origin
		Sound forcefieldactivate 90 90 .8
	End
End


Condition
	On	Time
	Time	0

	Event	
		Type	Local
		At	origin
		Sound pillboxforcefield_loop 90 90 .55
	End
End


Condition

	Event
		Type	local
		At	Origin
		Bhvr 	:PillBoxforceField.bhvr
		
		Geom	FX_PillBoxForceField
				
	End


End

Condition

	Event
		Type	local
		At	Origin

		Bhvr 	:PillBoxforceField.bhvr
		
		Geom	FX_PillBoxForceField
				
	End


End

Condition

	Event
		Type	local
		At	Origin

		Bhvr 	:PillBoxforceField.bhvr
		
		Geom	FX_PillBoxForceField
				
	End


End

End
