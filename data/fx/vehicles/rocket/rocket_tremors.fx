#########################################################
##	pump_smoke
##
FxInfo

Input
	InpName Origin
End

Condition
	Event
		At	Origin
		Type	local
		Bhvr	Vehicles/Rocket/Rocket_tremor.bhvr

		Lifespan 10
	End
End

Condition
	On	Cycle
	Time	10

	Event
		At	Origin
		Type	local
		Bhvr	Vehicles/Rocket/Rocket_tremor.bhvr
		Lifespan 10
	End
End

End			