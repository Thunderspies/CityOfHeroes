#########################################################
##	cot_waterfall_med.fx
##
FxInfo


###########################################################

Condition
	On	Time
	Time	2

	Event
		Type	local
		At	Root	
		
		Part	:MistHoldBlue1.part
		Part	:MistHoldPurple1.part
		Part	:MistHoldPurple1Flat.part
		Part	:MistHoldBlue1Flat.part
		Part	:MistHoldPurpleBase.part
		Part	:MistHoldBlueBase.part
		Sound Warshadehit1 80 80 .9

		

	End

	Event
		Type	local
		At	Root
		BHVR	:ImobilizeSpin.bhvr
	
		Part	:SwirllingSmoke.part
		Sound WarHold_loop 60 60 .66

	End

	Event
		Ename	GravitationalForce
		Type	Local
		At	Root
		
		BhvrOverride
			PhysForceType		In
			PhysForceRadius		15
			PhysForcePower		50
			PhysForcePowerJitter	10
		End

	End
End

End			