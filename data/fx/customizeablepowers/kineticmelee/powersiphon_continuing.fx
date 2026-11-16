#############################################################
## PowerSiphon_Continuing.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	RingsAnchor
		Type	Posit
		Sound KM_PowerSiphon_Continuing_Loop 200 200 .1
		At	Root
		Part	:Rings_Rising_LocalFacing_Smokey_Continuing.part
		Part	:Rings_Rising_LocalFacing_Continuing.part
	End
End

Condition
	On	Cycle
	Time	3

	Event
		EName	InnerTornado
		Type	PositOnly
		At	RingsAnchor
		BhvrOverride
			PyrRotateJitter	20 20 20
		End
		Part	:Rings_Rising_LocalFacing.part
		Lifespan 5
	End

	Event
		EName	InnerTornado
		Type	PositOnly
		At	RingsAnchor
		BhvrOverride
			PyrRotateJitter	20 20 20
		End
		Part	:Rings_Rising_LocalFacing_Smokey.part
		Lifespan 5
	End
End

#############################################################

End