#############################################################
## Child_QuickStrike_Right.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	PositOnly
		At	UArmR
		Part	:Beam_Rings.part
		POther	LArmR
		Lifespan	20
	End
End

Condition
	On	Time
	Time	8

	Event
		Type	Local
		At	HandR
		BhvrOverride
			PyrRotate	0 0 90
		End
		Part	:Hand_Trails.part
		Lifespan	10
	End

	Event
		Type	Local
		At	HandR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.5 0.0
			PyrRotate	60 30 30
			PyrRotateJitter 10 0 10
		End
		Part	:Hand_Rings_Burst_LocalFacing.part
		Part	:Beam_Rings.part
		Lifespan 	15
	End

	Event
		Type	Local
		At	HandR
		BhvrOverride
			PositionOffset	0.0 -0.5 0.0
			PyrRotate	100 0 -30
			PyrRotateJitter 10 0 10
		End
		Part	:Hand_Rings_Burst_LocalFacing.part
		Part	:Beam_Rings.part
		Lifespan 	14
	End

	Event
		Type	Local
		At	HandR
		BhvrOverride
			PositionOffset	0.0 -0.5 0.0
			PyrRotate	0 120 0
			PyrRotateJitter 20 0 20
		End
		Part	:Hand_Rings_Burst_LocalFacing.part
		Part	:Beam_Rings.part
		Lifespan 	13
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	WepR
		Part	:Rings_Burst_Rapid_Small.part
		Lifespan 	3
	End
End

Condition
	On	Time
	Time	13

	Event
		Type	Local
		At	WepR
		Part	:Rings_Shockwave_Small.part
		Lifespan 	3
	End
End

#############################################################

End