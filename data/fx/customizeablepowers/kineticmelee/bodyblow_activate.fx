#############################################################
## Child_QuickStrike_Right.fx
#############################################################

FxInfo
Lifespan 80

#############################################################
Condition
	Event
		Type	Local
		At	HandR
		Sound KM_BodyBlow_Attack_01 200 200 .8
	End
End

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	HandR
		Part	:Hand_Trails.part
		Part	:Hand_Trails_Rings_Small.part
		Lifespan	31
	End

	Event
		Type	Local
		At	HandL
		Part	:Hand_Trails.part
		Part	:Hand_Trails_Rings_Small.part
		Lifespan	31
	End
End

#############################################################
Condition
	On	Time
	Time	26

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

#############################################################

Condition
	On	Time
	Time	28

	Event
		Type	Local
		At	HandR
		Part	:Rings_Burst_Rapid_Small.part
		Lifespan 	3
	End

	Event
		Type	Local
		At	HandR
		Part	:Hand_Trails_Big.part
		Part	:Hand_Streak.part
		Lifespan 	7
	End
End

Condition
	On	Time
	Time	34

	Event
		Type	Local
		At	HandR
		Part	:Rings_Shockwave_Small.part
		Lifespan 	4
	End
End

#############################################################

End