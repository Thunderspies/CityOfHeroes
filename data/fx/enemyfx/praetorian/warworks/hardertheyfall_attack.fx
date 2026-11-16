#############################################################
## Child_OrbitalLance_Attack.fx
#############################################################

FxInfo
LifeSpan 5000


#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time 	21

	Event
		EName	HipsAnchor
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset	-2.0 0.0 0.0
		End
		Sound 	WW_Explosion_01 400 400 1
	End

	Event
		Type	PositOnly
		At	HipsAnchor
		Bhvr	behaviors\CameraShake02.bhvr

		BhvrOverride
			PositionOffset	0.0 5.4 0.0
		End
		Part	:Explosion_Flash_Large.part
		Part	:SelfDestruct_Glow.part
		Part	:SelfDestruct_Shockwave_Large.part
		Part	:SelfDestruct_Glow_Lingering.part
		Part	:SelfDestruct_Streaks.part
		Lifespan 3
	End


	Event
		Type	StartPositOnly
		At	HipsAnchor
		Bhvr	behaviors\CameraShake01.bhvr

		BhvrOverride
			PositionOffset	0.0 3.5 0.0
		End
		Part	:SelfDestruct_Shockwave.part
		Part	:SelfDestruct_Glow_Flat.part
		Lifespan 3
	End

End

Condition
	On	Time
	Time	22

	Event
		Type	PositOnly
		At	HipsAnchor
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		BhvrOverride
			PositionOffset		0 0.3 0
			Scale			0.01 0.01 0.01
			ScaleRate		1.5 1.5 1.5
 			EndScale		2.25 2.25 2.25
		End
		Part	:FireRing_Small.part
		Part	:FireRing_Highlight.part
		Lifespan 60
	End

	Event
		Type	PositOnly
		At	HipsAnchor
		Part	:Explosion_Fire_Blue.part
		Part	:Explosion_Fire_Yellow.part
		Part	:Explosion_Fire_Yellow_Flat.part

		Lifespan 10
	End
End

Condition
	On	Time
	Time	24

	Event
		EName	Sigil
		Type	StartPositOnly
		At	HipsAnchor
		Bhvr	behaviors\Splat_RuneCircle.bhvr
		BhvrOverride
			Alpha		255
			FadeInLength	5
			FadeOutLength	40
			StartColor	20 20 20
			Scale		80.0 10.0 80.0
		End
		Splat	Energy_ConeB3.tga
		Lifespan 190
	End

	Event
		Type	StartPositOnly
		At	HipsAnchor
		BhvrOverride
			PositionOffset	0.0 5.0 0.0
		End
		Part	:SelfDestruct_Smoke_Lingering.part
		Part	:SelfDestruct_Smoke.part
		Lifespan 1
	End
End

#############################################################

Condition
	On	Time
	Time	24

	Event
		SoftwareOnly
		Ename	parts1
		Type	StartPositOnly
		At	Chest
		Geom	GEO_Head_Warworks_01
		Bhvr	Explosions\TrashExplosion\TrashDebrisFade.bhvr
		BhvrOverride
			#PositionOffset		0 3 0
			StartJitter		1 1 1

			StartColor		50 50 50

			PyrRotateJitter		180 180 180
			#FadeOutLength		130
			Gravity			0.05

			Scale			1.0 1.0 1.0

			Spin			0.0 0.0 0.0
			SpinJitter		0.35 0.35 0.35

			InitialVelocity		0.0 1.5 0.0
			InitialVelocityJitter	1.5 0.8 1.5

			Drag			0.01

		#	physDebris		1
			physSFriction 	 	1.6
			PhysDensity		2
			PhysScale		1.25 1.25 1.25

		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	5000
		LifespanJitter	150
	End

	Event
		SoftwareOnly
		Ename	parts1
		Type	StartPositOnly
		At	Chest
		Geom	GEO_LArmR_Warworks_01
		Bhvr	Explosions\TrashExplosion\TrashDebrisFade.bhvr
		BhvrOverride
			StartJitter		1 1 1
			StartColor		50 50 50
			PyrRotateJitter		180 180 180
			Gravity			0.05

			Scale			1.0 1.0 1.0

		#	Spin			0.0 0.0 0.0
			SpinJitter		0.02 0.02 0.02

			InitialVelocity		0.0 1.25 0.0
			InitialVelocityJitter	0.6 0.1 0.6

			Drag			0.01

		#	physDebris		1
			physSFriction 	 	1.6
			PhysDensity		2
			PhysScale		1.25 1.25 1.25

		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	5000
		LifespanJitter	150
	End

	Event
		SoftwareOnly
		Ename	parts1
		Type	StartPositOnly
		At	Chest
		Geom	GEO_Chest_Warworks_01
		Bhvr	Explosions\TrashExplosion\TrashDebrisFade.bhvr
		BhvrOverride
			StartJitter		1 1 1
			StartColor		50 50 50
			PyrRotateJitter		180 180 180
			Gravity			0.05

			Scale			1.0 1.0 1.0

		#	Spin			0.0 0.0 0.0
			SpinJitter		0.025 0.025 0.025

			InitialVelocity		0.0 1.25 0.0
			InitialVelocityJitter	0.6 0.1 0.6

			Drag			0.01

		#	physDebris		1
			physSFriction 	 	1.6
			PhysDensity		2
			PhysScale		1.25 1.25 1.25

		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	5000
		LifespanJitter	150
	End
End

#############################################################

Condition
	On	Time
	Time	24
	Repeat	10
Event
		SoftwareOnly
		Ename	parts1
		Type	StartPositOnly
		At	Chest

		Geom	Metal_Flat05_Alluminum
		Geom	Metal_Flat06_Alluminum
		Geom	Metal_Flat07_Alluminum
		Geom	Widget_15_Alluminum
		Geom	Widget_17_Alluminum
		Geom	Widget_18_Alluminum
		Geom	Widget_22_Alluminum
		Geom	Widget_26_Alluminum
		Bhvr	Explosions\TrashExplosion\TrashDebrisFade.bhvr
		BhvrOverride
			#PositionOffset		0 3 0
			StartJitter		1 1 1

			StartColor		150 120 100

			PyrRotateJitter		180 180 180
			#FadeOutLength		130

			Scale			.5 .5 .5

			Spin			0.0 0.0 0.0
			SpinJitter		0.35 0.35 0.35

			InitialVelocity		0.0 1.5 0.0
			InitialVelocityJitter	1.5 0.8 1.5

			Drag			0.01

		#	physDebris		1
			physSFriction 	 	1.6
			PhysDensity		2
			PhysScale		1.25 1.25 1.25

		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	5000
		LifespanJitter	150
	End

End

#############################################################

Condition
	On	Time
	Time	29

	Event
		Type	Start
		At	HipsAnchor
		BhvrOverride
			PositionOffset	0.0 5.0 0.0
		End
		Part	:Globe_Sparks.part
		Part	:Globe_Cinders.part
		Lifespan 5
	End

End

#############################################################

End