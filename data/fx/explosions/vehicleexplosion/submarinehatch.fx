#########################################################
##	car on fire
#########################################################

FxInfo  
Condition

	Event
		EName 	SubHatch
		Type	local
		At	Chest
		BhvrOverride
			PositionOffset	0 10.85 -55
		End
		
	End

	Event
		EName 	SubHatchDoor
		Type	local
		At	Chest
		BhvrOverride
			PositionOffset	0 9.3 -52.4
		End
		
	End

########################################################################

	Event
		Ename	Rudder
		Type	local
		At	subhatch
		
		Geom	SewrHatch_Base
	End

	Event
		Ename	Rudder
		Type	local
		At	SubHatchDoor
		
		BhvrOverride
			PyrRotate	90 0 0
		End

		Geom	SewerAccess_Hatch

	End


End

End			