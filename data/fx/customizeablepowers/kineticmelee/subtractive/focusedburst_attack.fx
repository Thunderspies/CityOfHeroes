#############################################################
## ChargedBrawl_Attack.fx
#############################################################

FxInfo
Lifespan 80

#############################################################



Condition
	On Time
	Time 2
	Event
		Type Local
		At Mystic
		Sound KM_FocusedBurst_Attack_01 200 200 .8
	End
End

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	Mystic
		Bhvr	behaviors/ScaleBubble5.bhvr

		BhvrOverride
			Alpha		255
			Scale		0.125 0.125 0.125
			ScaleRate	0.125 0.125 0.125
			EndScale	0.3 0.3 0.3

			TintGeom	1
		End
		Geom	Tintable_Dark_SuperStrengthBubble
		Lifespan	25
	End

	Event
		Type	Local
		At	Mystic
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			Alpha		255
			Scale		0.125 0.125 0.125
			ScaleRate	0.125 0.125 0.125
			EndScale	0.3 0.3 0.3

			TintGeom	1
		End
		Part	:Sphere_Glow2.part
		Part	:Hand_Trails.part
		Part	:Sphere_Smoke.part
		Part	:Sphere_Glow.part
		Part	:Sphere_Perimeter.part
		Lifespan	10
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
		Lifespan 	30
	End

	Event
		Type	Local
		At	Mystic
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate	100 0 -30
			PyrRotateJitter 10 0 10

		End
		Part	:Rings_Burst_LocalFacing_Small.part
		Lifespan 	31
	End

	Event
		Type	Local
		At	Mystic
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate	0 120 0
			PyrRotateJitter 20 0 20

		End
		Part	:Rings_Burst_LocalFacing_Small.part
		Lifespan 	32
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	PositOnly
		At	Mystic
		Part	:Rings_Shrink_Smokey.part
		Part	:Rings_Shrink.part

		Lifespan	25
	End
End

Condition
	On	Time
	Time	48

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

	Event
		EName	Prime
		Type	Start
		At	WepR
		BhvrOverride
			Trackrate	3.0
			TintGeom	1
			Scale		0.125 0.125 0.125
		End
		Part	:Sphere_Smoke.part
		Part	:Sphere_Perimeter.part
		Part	:Rings_Burst_LocalFacing_Rapid.part
		Part	:Rings_Burst_LocalFacing_Rapid_Smokey.part
		Part	:Rings_Trail.part
		Part	:Projectile_Trail.part
		Geom	Tintable_Dark_SuperStrengthBubble
		Magnet	Target
		LookAt	Target
	End
End

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End
End

#############################################################

End