#############################################################
## ChargedBrawl_Attack.fx
#############################################################

FxInfo
Lifespan 120

#############################################################

Condition
	On	Time
	Time	10



	Event
		Type	Local
		At	HandR
		Sound KM_ConcentratedStrike_Attack_01 200 200 .8
		Part	:Hand_Trails.part
		Part	:Hand_Trails_Rings.part
		Lifespan	45
	End

	Event
		Type	Local
		At	HandL
		Part	:Hand_Trails.part
		Part	:Hand_Trails_Rings.part
		Lifespan	45
	End
End

Condition
	On 	Time
	Time 	25

	Event
		Type	Start
		At	Chest
		ChildFX	:GustTrail.fx
	End
End

Condition
	On 	Time
	Time 	40

	Event
		Type	Start
		At	Chest
		ChildFX	:GustTrail.fx
	End

End


Condition
	On	Time
	Time	55



	Event
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			Spin		0 1 0
			PyrRotate	90 30 30
			PyrRotateJitter 30 0 10
		End
		Part	:Rings_Suck_LocalFacing_Big.part
		Lifespan 	10
	End

	Event
		Type	PositOnly
		At	Chest
		BhvrOverride
			PyrRotate	-60 30 30
			PyrRotateJitter 10 0 10
		End
		Part	:Rings_Burst_LocalFacing_Big2.part
		Part	:Rings_Shockwave_Smokey_Sucking.part
		Lifespan 	5
	End
End

Condition
	On	Cycle
	Time	3

	Event
		Type	Local
		At	ShockwaveAnchor
		BhvrOverride
			PyrRotateJitter 180 180 180
		End
		Part	:Rings_Shockwave_big.part
		Lifespan 	20
	End
End

Condition
	On	Time
	Time	60



	Event
		Type	Local
		At	HandL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.5 0.0
			PyrRotate	60 30 30
			PyrRotateJitter 10 0 10
		End

		Part	:Hand_Rings_Burst_LocalFacing_Big2.part
		Part	:Beam_Rings.part
		Lifespan 	15
	End

	Event
		Type	Local
		At	HandL
		BhvrOverride
			PositionOffset	0.0 -0.5 0.0
			PyrRotate	100 0 -30
			PyrRotateJitter 10 0 10
		End
		Part	:Hand_Rings_Burst_LocalFacing_Big2.part
		Part	:Beam_Rings.part
		Lifespan 	14
	End

	Event
		Type	Local
		At	HandL
		BhvrOverride
			PositionOffset	0.0 -0.5 0.0
			PyrRotate	0 120 0
			PyrRotateJitter 20 0 20
		End
		Part	:Hand_Rings_Burst_LocalFacing_Big2.part
		Part	:Beam_Rings.part
		Lifespan 	13
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
		Part	:Hand_Rings_Burst_LocalFacing_Big2.part
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
		Part	:Hand_Rings_Burst_LocalFacing_Big2.part
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
		Part	:Hand_Rings_Burst_LocalFacing_Big2.part
		Part	:Beam_Rings.part
		Lifespan 	13
	End
End

#############################################################

Condition
	On	Time
	Time	75

	Event
		Type	Local
		At	HandL
		Part	:Rings_Burst_Rapid_Small.part
		Lifespan 	3
	End

	Event
		Type	Local
		At	HandL
		Part	:Hand_Trails_Big.part
		Part	:Hand_Streak.part
		Lifespan 	7
	End

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
	Time	80

	Event
		Type	Local
		At	HandL
		Part	:Rings_Shockwave.part
		Part	:Rings_Shockwave_Small.part
		Part	:Rings_Burst_Smokey.part

		Lifespan 	3
	End

	Event
		Type	Local
		At	HandR
		Part	:Rings_Shockwave.part
		Part	:Rings_Shockwave_Small.part
		Part	:Rings_Burst_Smokey.part

		Lifespan 	3
	End
End
#############################################################

End