#############################################################
## ChargedBrawl_Attack.fx
#############################################################

FxInfo
Lifespan 90

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	GalesAnchor
		Type	Start
		At	Root
		Lifespan 35
	End
End

Condition
	On	Time
	Time	5

	Event
		EName	RingsAnchor
		Type	Start
		At	Root
		Lifespan 60
	End

	Event
		EName	RingsAnchor
		Type	Posit
		At	Root
		Part	:Rings_Rising_LocalFacing_Smokey_Continuing.part
		Part	:Rings_Rising_LocalFacing_Continuing.part
	End
End

Condition
	On Time
	Time 10
	Event
		Type Local
		At Root
		Sound KM_PowerSiphon_Attack_01 200 200 .5
	End
End
	
Condition
	On 	Cycle
	Time 	10
	Repeat	2

	Event
		EName	InnerTornado
		Type	PositOnly
		At	GalesAnchor
		ChildFX :GustTrail_Small.fx
		Lifespan 60
	End
End

Condition
	On	Cycle
	Time	2

	Event
		EName	InnerTornado
		Type	PositOnly
		At	RingsAnchor
		BhvrOverride
			PyrRotateJitter	20 20 20
		End
		Part	:Rings_Rising_LocalFacing.part
		Lifespan 60
	End

	Event
		EName	InnerTornado
		Type	PositOnly
		At	RingsAnchor
		BhvrOverride
			PyrRotateJitter	20 20 20
		End
		Part	:Rings_Rising_LocalFacing_Smokey.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	25

	Event
		Type	Local
		At	WepL
		Part	:Hand_Trails.part
		Lifespan 	20
	End

	Event
		Type	Local
		At	WepL
		Bhvr	behaviors/ScaleBubble5.bhvr
		BhvrOverride
			Alpha		255
			TintGeom	1
		End
		Geom	Tintable_Dark_SuperStrengthBubble
		Lifespan	30
	End

	Event
		Type	Local
		At	WepL
		Part	:Sphere_Smoke.part
		Part	:Sphere_Glow.part
		Part	:Sphere_Perimeter.part
		Lifespan	20
	End

	Event
		Type	Local
		At	WepL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate	60 30 30
			PyrRotateJitter 10 0 10
		End
		Part	:Hand_Rings_Burst_LocalFacing_Big2.part
		Part	:Beam_Rings.part
		Lifespan 	19
	End

	Event
		Type	Local
		At	WepL
		BhvrOverride
			PyrRotate	100 0 -30
			PyrRotateJitter 10 0 10
		End
		Part	:Hand_Rings_Burst_LocalFacing_Big2.part
		Part	:Beam_Rings.part
		Lifespan 	17
	End

	Event
		Type	Local
		At	WepL
		BhvrOverride
			PyrRotate	0 120 0
			PyrRotateJitter 20 0 20
		End
		Part	:Hand_Rings_Burst_LocalFacing_Big2.part
		Part	:Beam_Rings.part
		Lifespan 	15
	End


End

#############################################################

Condition
	On	Time
	Time	30

	Event
		Type	Local
		At	WepL
		Part	:Rings_Burst_Rapid_Small.part
		Lifespan 	3
	End


End

#############################################################

End