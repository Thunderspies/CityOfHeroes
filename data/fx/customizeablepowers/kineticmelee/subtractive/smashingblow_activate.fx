#############################################################
## SmashingBlow_Activate.fx
#############################################################

FxInfo
Lifespan 80

#############################################################

Condition
	Event
		Type Local
		At Root
		Sound KM_SmashingBlow_Attack_01 200 200 .8
	End
End

Condition
	On	Time
	Time	5

	Event
		Ename	ConeOffset
		Type	Start
		At	Root
		BhvrOverride
			PYRRotate	0 180 0
			Scale		1.85 1.85 1.85
		End
		GEOM	FX_ConeOffsets
	End

	Event
		EName 	TargetOffset
		Type	Local
		At	ConeOffset
		BhvrOverride
			PositionOffset	0 2.0 0
		End
		AltPiv	1
	End

	Event
		Type	Local
		At	HandR
		Part	:Hand_Trails.part
		Part	:Hand_Trails_Rings.part
		Lifespan	25
	End

	Event
		Type	Local
		At	HandL
		Part	:Hand_Trails.part
		Part	:Hand_Trails_Rings.part
		Lifespan	25
	End
End

#############################################################

Condition
	On	Time
	Time	30


	Event
		Type	Local
		At	HandL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.5 0.0
			PyrRotate	60 30 30
			PyrRotateJitter 10 0 10
		End
		Part	:Hand_Rings_Burst_LocalFacing_Big.part
		Lifespan 	13
	End

	Event
		Type	Local
		At	HandL
		BhvrOverride
			PositionOffset	0.0 -0.5 0.0
			PyrRotate	100 0 -30
			PyrRotateJitter 10 0 10
		End
		Part	:Hand_Rings_Burst_LocalFacing_Big.part
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
		Part	:Hand_Rings_Burst_LocalFacing_Big.part
		Lifespan 	15
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
		Part	:Hand_Rings_Burst_LocalFacing_Big.part
		Lifespan 	13
	End

	Event
		Type	Local
		At	HandR
		BhvrOverride
			PositionOffset	0.0 -0.5 0.0
			PyrRotate	100 0 -30
			PyrRotateJitter 10 0 10
		End
		Part	:Hand_Rings_Burst_LocalFacing_Big.part
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
		Part	:Hand_Rings_Burst_LocalFacing_Big.part
		Lifespan 	15
	End
End

Condition
	On	Time
	Time	33

	Event
		Type	Local
		At	HandR
		Part	:Hand_Trails.part
		Part	:Hand_Trails_Rings.part
		Part	:Rings_Burst_Rapid_Small.part
		Lifespan	10
	End

	Event
		Type	Local
		At	HandL
		Part	:Hand_Trails.part
		Part	:Hand_Trails_Rings.part
		Part	:Rings_Burst_Rapid_Small.part
		Lifespan	10
	End
End

Condition
	On	Time
	Time	38

	Event
		Type	Start
		At	TargetOffset
		Part	:Gust_Streaks.part
		Part	:Gust_Rings.part
		PMagnet	Target
		Lifespan 15
	End
End

#############################################################

End