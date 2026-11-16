#############################################################
## ChargedBrawl_Attack.fx
#############################################################

FxInfo
Lifespan 80

#############################################################

Condition
	On Time
	Time 3
	Event
		Type Local
		At Mystic
		Sound 	KM_Burst_Attack_01 200 200 .8
	End
End

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	Mystic
		
		BhvrOverride
			Alpha		255
			Scale		0.125 0.125 0.125
			ScaleRate	0.125 0.125 0.125
			EndScale	0.3 0.3 0.3
			TintGeom	1
		End
		Part	:Sphere_Smoke.part
		Part	:Sphere_Glow.part
		Part	:Sphere_Glow2.part
		Part	:Hand_Trails.part
		Part	:Sphere_Perimeter.part
		Geom	Tintable_Dark_SuperStrengthBubble
		Lifespan	50
	End

	Event
		Type	Local
		At	Mystic
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate	60 30 30
			PyrRotateJitter 10 0 10
		End
		Part	:Rings_Burst_LocalFacing_Small.part

		Lifespan 	50
	End

	Event
		Type	Local
		At	Mystic
		BhvrOverride
			PyrRotate	100 0 -30
			PyrRotateJitter 10 0 10

		End
		Part	:Rings_Burst_LocalFacing_Small.part
		Lifespan 	50
	End

	Event
		Type	Local
		At	Mystic
		BhvrOverride
			PyrRotate	0 120 0
			PyrRotateJitter 20 0 20

		End
		Part	:Rings_Burst_LocalFacing_Small.part
		Lifespan 	50
	End
End

Condition
	On	TIme
	Time	20

	Event
		Type	PositOnly
		At	Mystic
		Part	:Rings_Shrink_Smokey.part
		Part	:Rings_Shrink.part

		Lifespan	30
	End
End
Condition
	On	Time
	Time	45

	Event
		Type	Local
		At	WepR
		BhvrOverride
			PyrRotate	0 0 90
		End
		Part	:Rings_Burst_Rapid.part
		Part	:Hand_Trails.part
		Lifespan 	5
	End
End

Condition
	On	Time
	Time	61


	Event
		Type	Start
		At	WepR
		Part	:Hand_Beam_Geyser.part
		Part	:Rings_Burst_LocalFacing.part
		Part	:Root_Ring_Burst.part
		Part	:Flash2.part
		Part	:Flash_Shockwave.part
		Part	:Flash_Core.part
		Lifespan 	5
	End

	Event
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 0.05 0
		End
		Part	:Flash2_Flat.part
		Lifespan 	5
	End

	Event
		Type	Local
		At	Root
		Bhvr	behaviors/ScaleBubble5.bhvr
		BhvrOverride
			Alpha		85
			TintGeom	1
		End
		Geom	Tintable_ElectricityHit
		Lifespan 5
	End

	Event
		Type	Start
		At	Chest
		BhvrOverride
			PositionOffset	0.0 1.0 0.0
			StartJitter	1.0 1.0 1.0
			PyrRotate	60 30 30
			PyrRotateJitter 10 0 10
		End
		Part	:Root_Ring_LocalFacing_Burst2.part
		Lifespan 	5
	End

	Event
		Type	Start
		At	Chest
		BhvrOverride
			PositionOffset	0.0 1.0 0.0
			StartJitter	1.0 1.0 1.0
			PyrRotate	100 0 -30
			PyrRotateJitter 10 0 10
		End
		Part	:Root_Ring_LocalFacing_Burst2.part
		Lifespan 	5
	End
End

Condition
	On	Time
	Time	63

	Event
		Type	Start
		At	Chest
		BhvrOverride
			PositionOffset	0.0 1.0 0.0
			StartJitter	1.0 1.0 1.0
			PyrRotate	180 -30 180
			PyrRotateJitter 10 0 10
		End
		Part	:Root_Ring_LocalFacing_Burst2.part
		Lifespan 	5
	End

	Event
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0.0 1.0 0.0
			StartJitter	1.0 1.0 1.0
		End
		Part	:Root_Ring_LocalFacing_Burst2.part
		Part	:Glow_Linger.part

		Lifespan 	5
	End
End

Condition
	On	Time
	Time	64

	Event
		Type	Start
		At	Chest
		BhvrOverride
			PositionOffset	0.0 1.0 0.0
			StartJitter	1.0 1.0 1.0
			PyrRotate	120 30 30
			PyrRotateJitter 10 0 10
		End
		Part	:Root_Ring_LocalFacing_Burst2.part
		Lifespan 	5
	End
End
#############################################################

End