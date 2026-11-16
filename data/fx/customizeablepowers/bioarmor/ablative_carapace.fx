#############################################################
## StoneArmor.fx
#############################################################

FxInfo

Flags    InheritAlpha InheritGeoScale

Lifespan 400

#############################################################
Condition
	On 	Time
	Time	6
	Event
		Type	Local
		At	Root
		Sound 	AblativeCarapace_01 100 100 .8
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	CHEST
		Bhvr 	:Ablative.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	FX_Chest_BioArmor_01
		Lifespan 30
	End

	Event
		Type	Local
		At	UARML
		Bhvr 	:Ablative.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	FX_UarmL_BioArmor_01
		Lifespan 30
	End

	Event
		Type	Local
		At	UARMR
		Bhvr 	:Ablative.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	FX_UarmR_BioArmor_01
		Lifespan 30
	End

	Event
		Type	Local
		At	UARML
		Bhvr 	:Ablative.bhvr
		BhvrOverride
			TintGeom	1
			PyrRotate 0 0 180
			#EndScale 4 4 4
		End
		geom	FX_UarmR_BioArmor_02
		Lifespan 30
	End
End

####################################################################

Condition
	On 	Time
	Time 	25

	Event
		Type	Local
		At	FootL
		Bhvr 	:Ablative.bhvr
		BhvrOverride
			TintGeom	1
			Scale 0.6 0.6 2
			FadeInLength 35
		End
		geom	FX_FootL_BioArmor_01
		Lifespan 20

	End

	Event
		Type	Local
		At	FootR
		Bhvr 	:Ablative.bhvr
		BhvrOverride
			TintGeom	1
			Scale 2 2 2
			FadeInLength 35
		End
		geom	FX_FootR_BioArmor_01
		Lifespan 20
	End
End

Condition
	On 	Time
	Time 	20

	Event
		Type	Local
		At	LLEGL
		Bhvr 	:Ablative.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	FX_LLEGL_BioArmor_01
		Lifespan 25
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr 	:Ablative.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	FX_LLEGR_BioArmor_01
		Lifespan 25
	End

	Event
		Type	Local
		At	UlegR
		Bhvr 	:Ablative.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	FX_UlegR_BioArmor_01
		Lifespan 25
	End

	Event
		Type	Local
		At	ULEGL
		Bhvr 	:Ablative.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	FX_ULEGL_BioArmor_01
		Lifespan 25
	End
End

Condition
	On 	Time
	Time 	45

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
		At	Head
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0 1 0
		End
		Part	:BodyGlow.part
		Part	:Dirt_Splatter.part
		Part	:Rocks_Out.part
		Part	:Dust_Out_01.part
		Part	:Dust_Out_02.part
		Lifespan 6
	End

	Event
		Ename 	BreakLarm1
		Type	Start
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:BodyGlow.part
		Part	:Dirt_Splatter.part
		Part	:Rocks_Out.part
		Part	:Dust_Out_01.part
		Part	:Dust_Out_02.part
		Lifespan 6
	End

	Event
		Ename 	BreakLarm1
		Type	Start
		At	UarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:BodyGlow.part
		Part	:Dirt_Splatter.part
		Part	:Rocks_Out.part
		Part	:Dust_Out_01.part
		Part	:Dust_Out_02.part
		Lifespan 6
	End

	Event
		Ename 	BreakLarm1
		Type	Start
		At	UarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:BodyGlow.part
		Part	:Dirt_Splatter.part
		Part	:Rocks_Out.part
		Part	:Dust_Out_01.part
		Part	:Dust_Out_02.part
		Lifespan 6
	End

	Event
		Ename 	BreakLarm1
		Type	Start
		At	LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:BodyGlow.part
		Part	:Dirt_Splatter.part
		Part	:Rocks_Out.part
		Part	:Dust_Out_01.part
		Part	:Dust_Out_02.part
		Lifespan 6
	End

	Event
		Ename 	BreakLarm1
		Type	Start
		At	LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:BodyGlow.part
		Part	:Dirt_Splatter.part
		Part	:Rocks_Out.part
		Part	:Dust_Out_01.part
		Part	:Dust_Out_02.part
		Lifespan 6
	End

	Event
		Ename 	BreakLarm1
		Type	Start
		At	UlegL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:BodyGlow.part
		Part	:Dirt_Splatter.part
		Part	:Rocks_Out.part
		Part	:Dust_Out_01.part
		Part	:Dust_Out_02.part
		Lifespan 6
	End

	Event
		Ename 	BreakLarm1
		Type	Start
		At	UlegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:BodyGlow.part
		Part	:Dirt_Splatter.part
		Part	:Rocks_Out.part
		Part	:Dust_Out_01.part
		Part	:Dust_Out_02.part
		Lifespan 6
	End

	Event
		Ename 	BreakLarm1
		Type	Start
		At	LlegL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:BodyGlow.part
		Part	:Dirt_Splatter.part
		Part	:Rocks_Out.part
		Part	:Dust_Out_01.part
		Part	:Dust_Out_02.part
		Lifespan 6
	End

	Event
		Ename 	BreakLarm1
		Type	Start
		At	LlegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:BodyGlow.part
		Part	:Dirt_Splatter.part
		Part	:Rocks_Out.part
		Part	:Dust_Out_01.part
		Part	:Dust_Out_02.part
		Lifespan 6
	End

	Event
		Ename 	BreakLarm1
		Type	Start
		At	FootL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:BodyGlow.part
		Part	:Dirt_Splatter.part
		Part	:Rocks_Out.part
		Part	:Dust_Out_01.part
		Part	:Dust_Out_02.part
		Lifespan 6
	End

	Event
		Ename 	BreakLarm1
		Type	Start
		At	FootR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:BodyGlow.part
		Part	:Dirt_Splatter.part
		Part	:Rocks_Out.part
		Part	:Dust_Out_01.part
		Part	:Dust_Out_02.part
		Lifespan 6
	End
End

Condition
	On 	Time
	Time 	46

## Chest

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
#
### L ARM
#
	Event
		Ename 	BreakLarm1
		Type	Start
		At	UarmL
		BhvrOverride
			TintGeom		1
			Physics			1
			PositionOffset		-2 0 0
			PyrRotate		5 0 5
			StartJitter		0.5 0.5	0.5
			Scale 			2 1.5 2
			InitialVelocity		0.02 0.15 0.04
			InitialVelocityJitter	0.05 0.08 0.02
			Gravity			0.04
			Drag			0.05
			Spin			0 -0.05 0
			SpinJitter		0 0.04 0.3
			FadeOutLength		45
			physGravity		0.6
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
		End
		Geom	FX_BioArmor_Break_01
		Geom	FX_BioArmor_Break_02
		Geom	FX_BioArmor_Break_03
		Geom	FX_BioArmor_Break_04
		Geom	FX_BioArmor_Break_05
		Geom	FX_BioArmor_Break_06
		Geom	FX_BioArmor_Break_07
		Geom	FX_BioArmor_Break_08
		Geom	FX_BioArmor_Break_09
		Geom	FX_BioArmor_Break_10
		Geom	FX_BioArmor_Break_11
		Geom	FX_BioArmor_Break_12
		Lifespan 200
	End

	Event
		Ename 	BreakLarm2
		Type	Start
		At	UarmL
		BhvrOverride
			TintGeom		1
			Physics			1
			PositionOffset		-2.5 -1 0
			PyrRotate		5 180 5
			StartJitter		0.5 0.5	0.5
			Scale 			1.5 2 2
			InitialVelocity		-0.15 0.15 0.04
			InitialVelocityJitter	0.08 0.08 0.02
			Gravity			0.04
			Drag			0.02
			Spin			0 -0.05 0
			SpinJitter		0 0.04 0.3
			FadeOutLength		45
			physGravity		0.6
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
		End
		Geom	FX_BioArmor_Break_01
		Geom	FX_BioArmor_Break_02
		Geom	FX_BioArmor_Break_03
		Geom	FX_BioArmor_Break_04
		Geom	FX_BioArmor_Break_05
		Geom	FX_BioArmor_Break_06
		Geom	FX_BioArmor_Break_07
		Geom	FX_BioArmor_Break_08
		Geom	FX_BioArmor_Break_09
		Geom	FX_BioArmor_Break_10
		Geom	FX_BioArmor_Break_11
		Geom	FX_BioArmor_Break_12
		Lifespan 200
	End

	Event
		Ename 	BreakLarm3
		Type	Start
		At	LarmL
		BhvrOverride
			TintGeom		1
			Physics			1
			PositionOffset		-2 0 0
			PyrRotate		5 0 5
			StartJitter		0.5 0.5	0.5
			Scale 			2 1.5 2
			InitialVelocity		0.02 0.15 0.04
			InitialVelocityJitter	0.05 0.08 0.02
			Gravity			0.04
			Drag			0.05
			Spin			0 -0.05 0
			SpinJitter		0 0.04 0.3
			FadeOutLength		45
			physGravity		0.6
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
		End
		Geom	FX_BioArmor_Break_01
		Geom	FX_BioArmor_Break_02
		Geom	FX_BioArmor_Break_03
		Geom	FX_BioArmor_Break_04
		Geom	FX_BioArmor_Break_05
		Geom	FX_BioArmor_Break_06
		Geom	FX_BioArmor_Break_07
		Geom	FX_BioArmor_Break_08
		Geom	FX_BioArmor_Break_09
		Geom	FX_BioArmor_Break_10
		Geom	FX_BioArmor_Break_11
		Geom	FX_BioArmor_Break_12
		Lifespan 200
	End

	Event
		Ename 	BreakLarm4
		Type	Start
		At	LarmL
		BhvrOverride
			TintGeom		1
			Physics			1
			PositionOffset		-2.5 -1 0
			PyrRotate		5 180 5
			StartJitter		0.5 0.5	0.5
			Scale 			1.5 2 2
			InitialVelocity		-0.15 0.15 0.04
			InitialVelocityJitter	0.08 0.08 0.02
			Gravity			0.04
			Drag			0.02
			Spin			0 -0.05 0
			SpinJitter		0 0.04 0.3
			FadeOutLength		45
			physGravity		0.6
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
		End
		Geom	FX_BioArmor_Break_01
		Geom	FX_BioArmor_Break_02
		Geom	FX_BioArmor_Break_03
		Geom	FX_BioArmor_Break_04
		Geom	FX_BioArmor_Break_05
		Geom	FX_BioArmor_Break_06
		Geom	FX_BioArmor_Break_07
		Geom	FX_BioArmor_Break_08
		Geom	FX_BioArmor_Break_09
		Geom	FX_BioArmor_Break_10
		Geom	FX_BioArmor_Break_11
		Geom	FX_BioArmor_Break_12
		Lifespan 200
	End

## R ARM

	Event
		Ename 	BreakRarm1
		Type	Start
		At	UarmR
		BhvrOverride
			TintGeom		1
			Physics			1
			PositionOffset		2 0 0
			PyrRotate		5 0 5
			StartJitter		0.5 0.5	0.5
			Scale 			2 1.5 2
			InitialVelocity		-0.02 0.3 0.04
			InitialVelocityJitter	0.05 0.08 0.02
			Gravity			0.04
			Drag			0.05
			Spin			0 -0.05 0
			SpinJitter		0 0.04 0.3
			FadeOutLength		45
			physGravity		0.6
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
		End
		Geom	FX_BioArmor_Break_01
		Geom	FX_BioArmor_Break_02
		Geom	FX_BioArmor_Break_03
		Geom	FX_BioArmor_Break_04
		Geom	FX_BioArmor_Break_05
		Geom	FX_BioArmor_Break_06
		Geom	FX_BioArmor_Break_07
		Geom	FX_BioArmor_Break_08
		Geom	FX_BioArmor_Break_09
		Geom	FX_BioArmor_Break_10
		Geom	FX_BioArmor_Break_11
		Geom	FX_BioArmor_Break_12
		Lifespan 200
	End

	Event
		Ename 	BreakRarm2
		Type	Start
		At	UarmR
		BhvrOverride
			TintGeom		1
			Physics			1
			PositionOffset		2.5 -1 0
			PyrRotate		5 180 5
			StartJitter		0.5 0.5	0.5
			Scale 			1.5 2 2
			InitialVelocity		0.2 0.3 0.04
			InitialVelocityJitter	0.08 0.08 0.02
			Gravity			0.04
			Drag			0.02
			Spin			0 -0.05 0
			SpinJitter		0 0.04 0.3
			FadeOutLength		45
			physGravity		0.6
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
		End
		Geom	FX_BioArmor_Break_01
		Geom	FX_BioArmor_Break_02
		Geom	FX_BioArmor_Break_03
		Geom	FX_BioArmor_Break_04
		Geom	FX_BioArmor_Break_05
		Geom	FX_BioArmor_Break_06
		Geom	FX_BioArmor_Break_07
		Geom	FX_BioArmor_Break_08
		Geom	FX_BioArmor_Break_09
		Geom	FX_BioArmor_Break_10
		Geom	FX_BioArmor_Break_11
		Geom	FX_BioArmor_Break_12
		Lifespan 200
	End

	Event
		Ename 	BreakLRarm3
		Type	Start
		At	LarmR
		BhvrOverride
			TintGeom		1
			Physics			1
			PositionOffset		2 0 0
			PyrRotate		5 0 5
			StartJitter		0.5 0.5	0.5
			Scale 			2 1.5 2
			InitialVelocity		-0.02 0.3 0.04
			InitialVelocityJitter	0.05 0.08 0.02
			Gravity			0.04
			Drag			0.05
			Spin			0 -0.05 0
			SpinJitter		0 0.04 0.3
			FadeOutLength		45
			physGravity		0.6
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
		End
		Geom	FX_BioArmor_Break_01
		Geom	FX_BioArmor_Break_02
		Geom	FX_BioArmor_Break_03
		Geom	FX_BioArmor_Break_04
		Geom	FX_BioArmor_Break_05
		Geom	FX_BioArmor_Break_06
		Geom	FX_BioArmor_Break_07
		Geom	FX_BioArmor_Break_08
		Geom	FX_BioArmor_Break_09
		Geom	FX_BioArmor_Break_10
		Geom	FX_BioArmor_Break_11
		Geom	FX_BioArmor_Break_12
		Lifespan 200
	End

	Event
		Ename 	BreakRarm4
		Type	Start
		At	LarmR
		BhvrOverride
			TintGeom		1
			Physics			1
			PositionOffset		2.5 -1 0
			PyrRotate		5 180 5
			StartJitter		0.5 0.5	0.5
			Scale 			1.5 2 2
			InitialVelocity		0.2 0.3 0.04
			InitialVelocityJitter	0.08 0.08 0.02
			Gravity			0.04
			Drag			0.02
			Spin			0 -0.05 0
			SpinJitter		0 0.04 0.3
			FadeOutLength		45
			physGravity		0.6
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
		End
		Geom	FX_BioArmor_Break_01
		Geom	FX_BioArmor_Break_02
		Geom	FX_BioArmor_Break_03
		Geom	FX_BioArmor_Break_04
		Geom	FX_BioArmor_Break_05
		Geom	FX_BioArmor_Break_06
		Geom	FX_BioArmor_Break_07
		Geom	FX_BioArmor_Break_08
		Geom	FX_BioArmor_Break_09
		Geom	FX_BioArmor_Break_10
		Geom	FX_BioArmor_Break_11
		Geom	FX_BioArmor_Break_12
		Lifespan 200
	End

## L LEG

	Event
		Ename 	BreakLleg1
		Type	Start
		At	UlegL
		BhvrOverride
			TintGeom		1
			PositionOffset		-2 0 0
			PyrRotate		5 0 5
			StartJitter		0.5 0.5	0.5
			Scale 			3 2 3
			InitialVelocity		0.02 0.1 0.04
			InitialVelocityJitter	0.05 0.08 0.02
			Gravity			0.04
			Drag			0.05
			Spin			0 -0.05 0
			SpinJitter		0 0.04 0.3
			FadeOutLength		15
		End
		Geom	FX_BioArmor_Break_01
		Geom	FX_BioArmor_Break_02
		Geom	FX_BioArmor_Break_03
		Geom	FX_BioArmor_Break_04
		Geom	FX_BioArmor_Break_05
		Geom	FX_BioArmor_Break_06
		Geom	FX_BioArmor_Break_07
		Geom	FX_BioArmor_Break_08
		Geom	FX_BioArmor_Break_09
		Geom	FX_BioArmor_Break_10
		Geom	FX_BioArmor_Break_11
		Geom	FX_BioArmor_Break_12
	End

	Event
		Ename 	BreakLleg2
		Type	Start
		At	UlegL
		BhvrOverride
			TintGeom		1
			PositionOffset		-2.5 -1 0
			PyrRotate		5 180 5
			StartJitter		0.5 0.5	0.5
			Scale 			2 3 3
			InitialVelocity		-0.2 0.1 0.04
			InitialVelocityJitter	0.08 0.08 0.02
			Gravity			0.04
			Drag			0.02
			Spin			0 -0.05 0
			SpinJitter		0 0.04 0.3
			FadeOutLength		15
		End
		Geom	FX_BioArmor_Break_01
		Geom	FX_BioArmor_Break_02
		Geom	FX_BioArmor_Break_03
		Geom	FX_BioArmor_Break_04
		Geom	FX_BioArmor_Break_05
		Geom	FX_BioArmor_Break_06
		Geom	FX_BioArmor_Break_07
		Geom	FX_BioArmor_Break_08
		Geom	FX_BioArmor_Break_09
		Geom	FX_BioArmor_Break_10
		Geom	FX_BioArmor_Break_11
		Geom	FX_BioArmor_Break_12
	End

	Event
		Ename 	BreakLleg3
		Type	Start
		At	LlegL
		BhvrOverride
			TintGeom		1
			PositionOffset		-2 0 0
			PyrRotate		5 0 5
			StartJitter		0.5 0.5	0.5
			Scale 			3 2 3
			InitialVelocity		0.3 0.1 0.04
			InitialVelocityJitter	0.05 0.08 0.02
			Gravity			0.04
			Drag			0.05
			Spin			0 -0.05 0
			SpinJitter		0 0.04 0.3
			FadeOutLength		15
		End
		Geom	FX_BioArmor_Break_01
		Geom	FX_BioArmor_Break_02
		Geom	FX_BioArmor_Break_03
		Geom	FX_BioArmor_Break_04
		Geom	FX_BioArmor_Break_05
		Geom	FX_BioArmor_Break_06
		Geom	FX_BioArmor_Break_07
		Geom	FX_BioArmor_Break_08
		Geom	FX_BioArmor_Break_09
		Geom	FX_BioArmor_Break_10
		Geom	FX_BioArmor_Break_11
		Geom	FX_BioArmor_Break_12
	End

	Event
		Ename 	BreakLleg4
		Type	Start
		At	LlegL
		BhvrOverride
			TintGeom		1
			PositionOffset		-2.5 -1 0
			PyrRotate		5 180 5
			StartJitter		0.5 0.5	0.5
			Scale 			2 3 3
			InitialVelocity		-0.2 0.1 0.04
			InitialVelocityJitter	0.08 0.08 0.02
			Gravity			0.04
			Drag			0.02
			Spin			0 -0.05 0
			SpinJitter		0 0.04 0.3
			FadeOutLength		15
		End
		Geom	FX_BioArmor_Break_01
		Geom	FX_BioArmor_Break_02
		Geom	FX_BioArmor_Break_03
		Geom	FX_BioArmor_Break_04
		Geom	FX_BioArmor_Break_05
		Geom	FX_BioArmor_Break_06
		Geom	FX_BioArmor_Break_07
		Geom	FX_BioArmor_Break_08
		Geom	FX_BioArmor_Break_09
		Geom	FX_BioArmor_Break_10
		Geom	FX_BioArmor_Break_11
		Geom	FX_BioArmor_Break_12
	End

## R leg

	Event
		Ename 	BreakRleg1
		Type	Start
		At	UlegR
		BhvrOverride
			TintGeom		1
			PositionOffset		2 0 0
			PyrRotate		5 0 5
			StartJitter		0.5 0.5	0.5
			Scale 			3 2 3
			InitialVelocity		-0.02 0.1 0.04
			InitialVelocityJitter	0.05 0.08 0.02
			Gravity			0.04
			Drag			0.05
			Spin			0 -0.05 0
			SpinJitter		0 0.04 0.3
			FadeOutLength		15
		End
		Geom	FX_BioArmor_Break_01
		Geom	FX_BioArmor_Break_02
		Geom	FX_BioArmor_Break_03
		Geom	FX_BioArmor_Break_04
		Geom	FX_BioArmor_Break_05
		Geom	FX_BioArmor_Break_06
		Geom	FX_BioArmor_Break_07
		Geom	FX_BioArmor_Break_08
		Geom	FX_BioArmor_Break_09
		Geom	FX_BioArmor_Break_10
		Geom	FX_BioArmor_Break_11
		Geom	FX_BioArmor_Break_12
	End

	Event
		Ename 	BreakRleg2
		Type	Start
		At	UlegR
		BhvrOverride
			TintGeom		1
			PositionOffset		2.5 -1 0
			PyrRotate		5 180 5
			StartJitter		0.5 0.5	0.5
			Scale 			2 3 3
			InitialVelocity		0.2 0.1 0.04
			InitialVelocityJitter	0.08 0.08 0.02
			Gravity			0.04
			Drag			0.02
			Spin			0 -0.05 0
			SpinJitter		0 0.04 0.3
			FadeOutLength		15
		End
		Geom	FX_BioArmor_Break_01
		Geom	FX_BioArmor_Break_02
		Geom	FX_BioArmor_Break_03
		Geom	FX_BioArmor_Break_04
		Geom	FX_BioArmor_Break_05
		Geom	FX_BioArmor_Break_06
		Geom	FX_BioArmor_Break_07
		Geom	FX_BioArmor_Break_08
		Geom	FX_BioArmor_Break_09
		Geom	FX_BioArmor_Break_10
		Geom	FX_BioArmor_Break_11
		Geom	FX_BioArmor_Break_12
	End

	Event
		Ename 	BreakLRleg3
		Type	Start
		At	LlegR
		BhvrOverride
			TintGeom		1
			PositionOffset		2 0 0
			PyrRotate		5 0 5
			StartJitter		0.5 0.5	0.5
			Scale 			3 2 3
			InitialVelocity		-0.02 0.1 0.04
			InitialVelocityJitter	0.05 0.08 0.02
			Gravity			0.04
			Drag			0.05
			Spin			0 -0.05 0
			SpinJitter		0 0.04 0.3
			FadeOutLength		15
		End
		Geom	FX_BioArmor_Break_01
		Geom	FX_BioArmor_Break_02
		Geom	FX_BioArmor_Break_03
		Geom	FX_BioArmor_Break_04
		Geom	FX_BioArmor_Break_05
		Geom	FX_BioArmor_Break_06
		Geom	FX_BioArmor_Break_07
		Geom	FX_BioArmor_Break_08
		Geom	FX_BioArmor_Break_09
		Geom	FX_BioArmor_Break_10
		Geom	FX_BioArmor_Break_11
		Geom	FX_BioArmor_Break_12
	End

	Event
		Ename 	BreakLRleg4
		Type	Start
		At	LlegR
		BhvrOverride
			TintGeom		1
			PositionOffset		2.5 -1 0
			PyrRotate		5 180 5
			StartJitter		0.5 0.5	0.5
			Scale 			2 3 3
			InitialVelocity		0.12 0.1 0.04
			InitialVelocityJitter	0.08 0.08 0.02
			Gravity			0.04
			Drag			0.02
			Spin			0 -0.05 0
			SpinJitter		0 0.04 0.3
			FadeOutLength		15
		End
		Geom	FX_BioArmor_Break_01
		Geom	FX_BioArmor_Break_02
		Geom	FX_BioArmor_Break_03
		Geom	FX_BioArmor_Break_04
		Geom	FX_BioArmor_Break_05
		Geom	FX_BioArmor_Break_06
		Geom	FX_BioArmor_Break_07
		Geom	FX_BioArmor_Break_08
		Geom	FX_BioArmor_Break_09
		Geom	FX_BioArmor_Break_10
		Geom	FX_BioArmor_Break_11
		Geom	FX_BioArmor_Break_12
	End

End

#########################################################


End