#############################################################
## AssassinsStrike_Windup.fx
#############################################################

FxInfo
Lifespan 100

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	PositOnly
		At	WepL
		Part	:Hand_Trails_Rings.part
		Sound KM_AssassinsStrike_Attack_01 200 200 .8
		Lifespan	35
	End

	Event
		Type	PositOnly
		At	WepR
		Part	:Hand_Trails_Rings.part
		Lifespan	35
	End

	Event
		Type	PositOnly
		At	WepL
		Part	:Hand_Trails.part
		Lifespan	45
	End

	Event
		Type	PositOnly
		At	WepR
		Part	:Hand_Trails.part
		Lifespan	45
	End
End

Condition
	On	Time
	Time	46

	Event
		Type	Start
		At	WepR
		Bhvr	behaviors/ScaleBubble5.bhvr
		BhvrOverride
			PositionOffset	0 -0.2 0
		End
		Geom	Tintable_SuperStrengthBubble

		Lifespan	33
	End

	Event
		Type	Start
		At	WepR
		BhvrOverride
			PositionOffset	0 -0.2 0
		End
		Part	:Rings_Shrink_Smokey.part
		Part	:Rings_Shrink.part
		Lifespan	25
	End


	Event
		Type	Start
		At	WepR
		BhvrOverride
			PositionOffset	0 -0.2 0
		End
		Part	:Hand_Trails.part
		Part	:Sphere_Smoke.part
		Part	:Sphere_Glow_Short.part
		Part	:Sphere_Perimeter.part

		Lifespan	5
	End

	Event
		Type	Start
		At	WepR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 -0.2 0
			PyrRotate	60 30 30
			PyrRotateJitter 10 0 10
		End
		Part	:Rings_Burst_LocalFacing_Small.part

		Lifespan 	31
	End

	Event
		Type	Start
		At	WepR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 -0.2 0
			PyrRotate	100 0 -30
			PyrRotateJitter 10 0 10

		End
		Part	:Rings_Burst_LocalFacing_Small.part
		Lifespan 	32
	End

	Event
		Type	Start
		At	WepR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 -0.2 0
			PyrRotate	0 120 0
			PyrRotateJitter 20 0 20
		End
		Part	:Rings_Burst_LocalFacing_Small.part
		Lifespan 	33
	End
End
#############################################################

End