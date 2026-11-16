#############################################################
## StoneArmor.fx
#############################################################

FxInfo

Flags    InheritAlpha InheritGeoScale

Lifespan 400

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	CHEST
		Bhvr 	:Ablative.bhvr
		BhvrOverride
			TintGeom	1
			EndScale	1.5 1.5 1.5
			PositionOffset	0 -0.4 0
		End
		geom	FX_Chest_BioArmor_01
		Lifespan 60
	End

	Event
		Type	Local
		At	CHEST
		Bhvr 	:Ablative.bhvr
		BhvrOverride
			TintGeom	1
			PyrRotate	0 180 0
			EndScale	1.5 1.5 1.5
			PositionOffset	0 -0.4 0
		End
		geom	FX_Chest_BioArmor_01
		Lifespan 60
	End
End

####################################################################

Condition
	On 	Time
	Time 	75

	Event
		Ename 	BreakAnchor
		Type	Local
		At	Chest
		BhvrOverride
			Scale 		2 2 2
			Alpha 1
		End
		Geom	FX_BioArmor_BreakOffset
	End

	Event
		Ename 	BreakLarm1
		Type	Start
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:BodyGlow.part
	#	Part	:Dirt_Splatter.part
		Part	:Rocks_Out.part
		Part	:Rocks_Out.part
		Part	:Dust_Out_01.part
		Part	:Dust_Out_02.part
		Lifespan 6
	End
End

Condition
	On 	Time
	Time 	76

	Event
		Ename 	Break1
		Type	Start
		At	BreakAnchor
		Altpiv 1
		BhvrOverride
			TintGeom		1
			physics 		1
			#Scale 			2 2 2
			InitialVelocity		0.0 0.1 0.04
			InitialVelocityJitter	0.05 0.08 0.02
			Gravity			0.04
			Drag			0.04
			Spin			0 -0.05 0
			SpinJitter		0 0.04 0.3
			FadeOutLength		45
			physGravity		1
			physRestitution 	0.2
			physSFriction 	 	0.9
			physDFriction 	 	0.9
		End
		Geom	FX_BioArmor_Break_01
		Lifespan 100
	End

	Event
		Ename 	Break2
		Type	Start
		At	BreakAnchor
		Altpiv 2
		BhvrOverride
			TintGeom		1
			physics 		1
			#Scale 			2 2 2
			InitialVelocity		0.04 0.15 0.04
			InitialVelocityJitter	0.05 0.08 0.02
			Gravity			0.04
			Drag			0.06
			Spin			0 -0.05 0
			SpinJitter		0 0.04 0.3
			FadeOutLength		45
			physGravity		1
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
		End
		Geom	FX_BioArmor_Break_02
		Lifespan 100
	End

	Event
		Ename 	Break3
		Type	Start
		At	BreakAnchor
		Altpiv 3
		BhvrOverride
			TintGeom		1
			physics 		1
			#Scale 			2 2 2
			InitialVelocity		0.0 0.2 0.04
			InitialVelocityJitter	0.05 0.08 0.02
			Gravity			0.04
			Drag			0.04
			Spin			0 -0.05 0
			SpinJitter		0 0.04 0.3
			FadeOutLength		45
			physGravity		0.6
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
		End
		Geom	FX_BioArmor_Break_03
		Lifespan 100
	End

	Event
		Ename 	Break4
		Type	Start
		At	BreakAnchor
		Altpiv 4
		BhvrOverride
			TintGeom		1
			physics 		1
			#Scale 			2 2 2
			InitialVelocity		0.0 0.2 -0.04
			InitialVelocityJitter	0.05 0.08 0.02
			Gravity			0.04
			Drag			0.04
			Spin			0 -0.05 0
			SpinJitter		0 0.04 0.3
			FadeOutLength		45
			physGravity		0.6
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
		End
		Geom	FX_BioArmor_Break_04
		Lifespan 100
	End

	Event
		Ename 	Break5
		Type	Start
		At	BreakAnchor
		Altpiv 5
		BhvrOverride
			TintGeom	1
			#Scale 		2 2 2
			InitialVelocity		0.0 0.2 -0.04
			InitialVelocityJitter	0.05 0.08 0.02
			Gravity			0.1
			Drag			0.04
			Spin			0 -0.05 0
			SpinJitter		0 0.04 0.3
			FadeOutLength		45
			physGravity		0.6
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
		End
		Geom	FX_BioArmor_Break_05
		Lifespan 100
	End

	Event
		Ename 	Break6
		Type	Start
		At	BreakAnchor
		Altpiv 6
		BhvrOverride
			TintGeom	1
			#Scale 		2 2 2
			InitialVelocity		0.0 0.2 0.04
			InitialVelocityJitter	0.05 0.08 0.02
			Gravity			0.02
			Drag			0.06
			Spin			0 -0.05 0
			SpinJitter		0 0.04 0.3
			FadeOutLength		45
			physGravity		0.6
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
		End
		Geom	FX_BioArmor_Break_06
		Lifespan 100
	End

	Event
		Ename 	Break7
		Type	Start
		At	BreakAnchor
		Altpiv 7
		BhvrOverride
			TintGeom	1
			#Scale 		2 2 2
			InitialVelocity		0.0 0.15 0.04
			InitialVelocityJitter	0.05 0.08 0.02
			Gravity			0.1
			Drag			0.06
			Spin			0 -0.05 0.05
			SpinJitter		0 0.04 0.1
			FadeOutLength		45
			physGravity		0.6
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
		End
		Geom	FX_BioArmor_Break_07
		Lifespan 100
	End

	Event
		Ename 	Break8
		Type	Start
		At	BreakAnchor
		Altpiv 8
		BhvrOverride
			TintGeom	1
			#Scale 		2 2 2
			InitialVelocity		-0.03 0.04 -0.04
			InitialVelocityJitter	0.05 0.08 0.02
			Gravity			0.04
			Drag			0.08
			Spin			0 0.03 0.05
			SpinJitter		0 0.04 0.2
			FadeOutLength		45
			physGravity		0.6
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
		End
		Geom	FX_BioArmor_Break_08
		Lifespan 100
	End

	Event
		Ename 	Break9
		Type	Start
		At	BreakAnchor
		Altpiv 9
		BhvrOverride
			TintGeom	1
			#Scale 		2 2 2
			InitialVelocity		0.15 0.04 -0.06
			InitialVelocityJitter	0.08 0.08 0.04
			Gravity			0.04
			Drag			0.08
			Spin			0 -0.03 -0.05
			SpinJitter		0 0.04 0.2
			FadeOutLength		45
			physGravity		0.6
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
		End
		Geom	FX_BioArmor_Break_09
		Lifespan 100
	End

	Event
		Ename 	Break10
		Type	Start
		At	BreakAnchor
		Altpiv 10
		BhvrOverride
			TintGeom	1
			#Scale 		2 2 2
			InitialVelocity		0.15 0.04 -0.02
			InitialVelocityJitter	0.08 0.08 0.05
			Gravity			0.03
			Drag			0.08
			Spin			0 -0.03 -0.05
			SpinJitter		0 0.04 0.2
			FadeOutLength		45
			physGravity		0.6
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
		End
		Geom	FX_BioArmor_Break_10
		Lifespan 100
	End

	Event
		Ename 	Break11
		Type	Start
		At	BreakAnchor
		Altpiv 11
		BhvrOverride
			TintGeom	1
			#Scale 		2 2 2
			InitialVelocity		0.05 0.04 0.02
			InitialVelocityJitter	0.05 0.08 0.05
			Gravity			0.04
			Drag			0.04
			Spin			0 0.02 0.05
			SpinJitter		0 0.03 0.1
			FadeOutLength		45
			physGravity		0.6
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
		End
		Geom	FX_BioArmor_Break_11
		Lifespan 100
	End

	Event
		Ename 	Break12
		Type	Start
		At	BreakAnchor
		Altpiv 12
		BhvrOverride
			TintGeom	1
			#Scale 		2 2 2
			InitialVelocity		-0.15 0.04 -0.05
			InitialVelocityJitter	0.08 0.08 0.02
			Gravity			0.04
			Drag			0.04
			Spin			0 0.02 0.05
			SpinJitter		0 0.03 0.1
			FadeOutLength		45
			physGravity		0.6
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
		End
		Geom	FX_BioArmor_Break_12
		Lifespan 100
	End

End

#########################################################


End