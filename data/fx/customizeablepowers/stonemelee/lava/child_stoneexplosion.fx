#############################################################
## StoneMeleeExplosion_Child.Fx
#############################################################

FxInfo

Lifespan 1150

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	start
		At	chest
		BhvrOverride
			FadeOutLength		1100
		End
		Part	CustomizeablePowers\StoneMelee\RockSparks.part
		Part	CustomizeablePowers\StoneMelee\RockExplosionBig.part
		Part	CustomizeablePowers\StoneMelee\RockExplosion01.part
		Part	CustomizeablePowers\StoneMelee\RockExplosion02.part
		Lifespan 5
	End
End

#############################################################

Condition
	Repeat	3
	RepeatJitter	2

	Event
		Type	start
		At	chest
		Bhvr	Powers\RockOffset2.bhvr
		BhvrOverride
			Scale			0.2 0.2 0.2
			Drag			0.004
			physics 		1
			physRadius 		0.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			StartColor		200 210 255
			PhysDebris		3
			PhysDensity		2
			Gravity			0.048
			TintGeom		1
		End
		Geom	Tintable_FX_Magma01
		Geom	Tintable_FX_Magma02
		Geom	Tintable_FX_Magma03
		Geom	Tintable_FX_Magma04
		Geom	Tintable_FX_Magma05
		Lifespan 1120
		LifespanJitter 130
	End
End

Condition
	Repeat	3
	RepeatJitter	2

	Event
		Type	start
		At	chest
		Bhvr	Powers\RockOffset3.bhvr
		BhvrOverride
			Scale			0.4 0.4 0.4
			Drag			0.004
			physics 		1
			physRadius 		0.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			StartColor		200 210 255
			PhysDebris		3
			PhysDensity		2
			Gravity			0.048
			TintGeom		1
		End
		Geom	Tintable_FX_Stone01
		Geom	Tintable_FX_Stone02
		Geom	Tintable_FX_Stone03
		Geom	Tintable_FX_Stone04
		Geom	Tintable_FX_Stone05
		Lifespan 1120
		LifespanJitter 130
	End
End

Condition
	Time 	0
	Chance	0.9

	Event
		Type	start
		At	chest
		Bhvr	Powers\RockOffset.bhvr
		BhvrOverride
			Scale			0.1 0.1 0.1
			Drag			0.004
			physics 		1
			physRadius 		0.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			StartColor		200 210 255
			PhysDebris		3
			PhysDensity		2
			Gravity			0.048
			TintGeom		1
			PrimaryTint		0
			PrimaryTint		100
		End
		Geom	Tintable_Cav_rockSM_01
		Geom	Tintable_Cav_rockSM_02
		Geom	Tintable_Cav_rockSM_03
		Geom	Tintable_cvsmhlls_rock_sml_01
		Geom	Tintable_cvsmhlls_rock_sml_02
		Geom	Tintable_cvsmhlls_rock_sml_03
		Lifespan 1120
		LifespanJitter 130
	End
End

Condition
	Time 	0
	Chance	0.9

	Event
		Type	start
		At	chest
		Bhvr	Powers\RockOffset2.bhvr
		BhvrOverride
			Scale			0.1 0.1 0.1
			Drag			0.004
			physics 		1
			physRadius 		0.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			StartColor		200 210 255
			PhysDebris		3
			PhysDensity		2
			Gravity			0.048
			TintGeom		1
		End
		Geom	Tintable_Cav_rockSM_01
		Geom	Tintable_Cav_rockSM_02
		Geom	Tintable_Cav_rockSM_03
		Geom	Tintable_cvsmhlls_rock_sml_01
		Geom	Tintable_cvsmhlls_rock_sml_02
		Geom	Tintable_cvsmhlls_rock_sml_03
		Lifespan 1120
		LifespanJitter 130
	End
End

Condition
	Time 	0
	Chance	0.9

	Event
		Type	start
		At	chest
		Bhvr	Powers\RockOffset3.bhvr
		BhvrOverride
			Scale			0.1 0.1 0.1
			Drag			0.004
			physics 		1
			physRadius 		0.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			StartColor		200 210 255
			PhysDebris		3
			PhysDensity		2
			Gravity			0.048
			TintGeom		1
		End
		Geom	Tintable_Cav_rockSM_01
		Geom	Tintable_Cav_rockSM_02
		Geom	Tintable_Cav_rockSM_03
		Geom	Tintable_cvsmhlls_rock_sml_01
		Geom	Tintable_cvsmhlls_rock_sml_02
		Geom	Tintable_cvsmhlls_rock_sml_03
		Lifespan 1120
		LifespanJitter 130
	End
End

Condition
	Time 	0
	Chance	0.9

	Event
		Type	start
		At	chest
		Bhvr	Powers\RockOffset4.bhvr
		BhvrOverride
			Scale			0.1 0.1 0.1
			Drag			0.004
			physics 		1
			physRadius 		0.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			StartColor		200 210 255
			PhysDebris		3
			PhysDensity		2
			Gravity			0.048
			TintGeom		1
		End
		Geom	Tintable_Cav_rockSM_01
		Geom	Tintable_Cav_rockSM_02
		Geom	Tintable_Cav_rockSM_03
		Geom	Tintable_cvsmhlls_rock_sml_01
		Geom	Tintable_cvsmhlls_rock_sml_02
		Geom	Tintable_cvsmhlls_rock_sml_03
		Lifespan 1120
		LifespanJitter 130
	End
End

Condition
	Time 	0
	Chance	0.9

	Event
		Type	start
		At	chest
		Bhvr	Powers\RockOffset5.bhvr
		BhvrOverride
			Scale			0.1 0.1 0.1
			Drag			0.004
			physics 		1
			physRadius 		0.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			StartColor		200 210 255
			PhysDebris		3
			PhysDensity		2
			Gravity			0.048
			TintGeom		1
		End
		Geom	Tintable_Cav_rockSM_01
		Geom	Tintable_Cav_rockSM_02
		Geom	Tintable_Cav_rockSM_03
		Geom	Tintable_cvsmhlls_rock_sml_01
		Geom	Tintable_cvsmhlls_rock_sml_02
		Geom	Tintable_cvsmhlls_rock_sml_03
		Lifespan 1120
		LifespanJitter 130
	End
End

#############################################################

Condition
	Time 	0
	Chance	0.9

	Event
		Type	start
		At	chest
		Bhvr	Powers\RockOffset.bhvr
		BhvrOverride
			Scale			0.1 0.1 0.1
			Drag			0.004
			physics 		1
			physRadius 		0.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			StartColor		200 210 255
			PhysDebris		3
			PhysDensity		2
			Gravity			0.048
			TintGeom		1
		End
		Geom	Tintable_Cav_rockSM_01
		Geom	Tintable_Cav_rockSM_02
		Geom	Tintable_Cav_rockSM_03
		Geom	Tintable_cvsmhlls_rock_sml_01
		Geom	Tintable_cvsmhlls_rock_sml_02
		Geom	Tintable_cvsmhlls_rock_sml_03
		Lifespan 1120
		LifespanJitter 130
	End
End

Condition
	Time 	0
	Chance	0.9

	Event
		Type	start
		At	chest
		Bhvr	Powers\RockOffset2.bhvr
		BhvrOverride
			Scale			0.1 0.1 0.1
			Drag			0.004
			physics 		1
			physRadius 		0.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			StartColor		200 210 255
			PhysDebris		3
			PhysDensity		2
			Gravity			0.048
			TintGeom		1
		End
		Geom	Tintable_Cav_rockSM_01
		Geom	Tintable_Cav_rockSM_02
		Geom	Tintable_Cav_rockSM_03
		Geom	Tintable_cvsmhlls_rock_sml_01
		Geom	Tintable_cvsmhlls_rock_sml_02
		Geom	Tintable_cvsmhlls_rock_sml_03
		Lifespan 1120
		LifespanJitter 130
	End
End

Condition
	Time 	0
	Chance	0.9

	Event
		Type	start
		At	chest
		Bhvr	Powers\RockOffset3.bhvr
		BhvrOverride
			Scale			0.1 0.1 0.1
			Drag			0.004
			physics 		1
			physRadius 		0.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			StartColor		200 210 255
			PhysDebris		3
			PhysDensity		2
			Gravity			0.048
			TintGeom		1
		End
		Geom	Tintable_Cav_rockSM_01
		Geom	Tintable_Cav_rockSM_02
		Geom	Tintable_Cav_rockSM_03
		Geom	Tintable_cvsmhlls_rock_sml_01
		Geom	Tintable_cvsmhlls_rock_sml_02
		Geom	Tintable_cvsmhlls_rock_sml_03
		Lifespan 1120
		LifespanJitter 130
	End
End

Condition
	Time 	0
	Chance	0.9

	Event
		Type	start
		At	chest
		Bhvr	Powers\RockOffset4.bhvr
		BhvrOverride
			Scale			0.1 0.1 0.1
			Drag			0.004
			physics 		1
			physRadius 		0.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			StartColor		200 210 255
			PhysDebris		3
			PhysDensity		2
			Gravity			0.048
			TintGeom		1
		End
		Geom	Tintable_Cav_rockSM_01
		Geom	Tintable_Cav_rockSM_02
		Geom	Tintable_Cav_rockSM_03
		Geom	Tintable_cvsmhlls_rock_sml_01
		Geom	Tintable_cvsmhlls_rock_sml_02
		Geom	Tintable_cvsmhlls_rock_sml_03
		Lifespan 1120
		LifespanJitter 130
	End
End

Condition
	Time 	0
	Chance	0.9

	Event
		Type	start
		At	chest
		Bhvr	Powers\RockOffset5.bhvr
		BhvrOverride
			Scale			0.1 0.1 0.1
			Drag			0.004
			physics 		1
			physRadius 		0.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			StartColor		200 210 255
			PhysDebris		3
			PhysDensity		2
			Gravity			0.048
			TintGeom		1
		End
		Geom	Tintable_Cav_rockSM_01
		Geom	Tintable_Cav_rockSM_02
		Geom	Tintable_Cav_rockSM_03
		Geom	Tintable_cvsmhlls_rock_sml_01
		Geom	Tintable_cvsmhlls_rock_sml_02
		Geom	Tintable_cvsmhlls_rock_sml_03
		Lifespan 1120
		LifespanJitter 130
	End
End

#############################################################

End