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
		Part	Explosions\StoneStatueExplosions\RockSparks.part
		Part	Explosions\StoneStatueExplosions\RockExplosionBig.part
		Part	Explosions\StoneStatueExplosions\RockExplosion01.part
		Part	Explosions\StoneStatueExplosions\RockExplosion02.part
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
		Bhvr	:RockOffset2.bhvr
		BhvrOverride
			Scale			0.15 0.15 0.15
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
		End
		Geom	FX_Stone02
		Geom	FX_Stone02
		Geom	FX_Stone02
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
		Bhvr	:RockOffset3.bhvr
		BhvrOverride
			Scale			0.25 0.25 0.25
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
		End
		Geom	FX_Stone02
		Geom	FX_Stone02
		Geom	FX_Stone02
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
		Bhvr	:RockOffset.bhvr
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
		End
		Geom	Cav_rockSM_01
		Geom	Cav_rockSM_02
		Geom	Cav_rockSM_03
		Geom	_cvsmhlls_rock_sml_01
		Geom	_cvsmhlls_rock_sml_02
		Geom	_cvsmhlls_rock_sml_03
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
		Bhvr	:RockOffset2.bhvr
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
		End
		Geom	Cav_rockSM_01
		Geom	Cav_rockSM_02
		Geom	Cav_rockSM_03
		Geom	_cvsmhlls_rock_sml_01
		Geom	_cvsmhlls_rock_sml_02
		Geom	_cvsmhlls_rock_sml_03
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
		Bhvr	:RockOffset3.bhvr
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
		End
		Geom	Cav_rockSM_01
		Geom	Cav_rockSM_02
		Geom	Cav_rockSM_03
		Geom	_cvsmhlls_rock_sml_01
		Geom	_cvsmhlls_rock_sml_02
		Geom	_cvsmhlls_rock_sml_03
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
		Bhvr	:RockOffset4.bhvr
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
		End
		Geom	Cav_rockSM_01
		Geom	Cav_rockSM_02
		Geom	Cav_rockSM_03
		Geom	_cvsmhlls_rock_sml_01
		Geom	_cvsmhlls_rock_sml_02
		Geom	_cvsmhlls_rock_sml_03
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
		Bhvr	:RockOffset5.bhvr
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
		End
		Geom	Cav_rockSM_01
		Geom	Cav_rockSM_02
		Geom	Cav_rockSM_03
		Geom	_cvsmhlls_rock_sml_01
		Geom	_cvsmhlls_rock_sml_02
		Geom	_cvsmhlls_rock_sml_03
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
		Bhvr	:RockOffset.bhvr
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
		End
		Geom	Cav_rockSM_01
		Geom	Cav_rockSM_02
		Geom	Cav_rockSM_03
		Geom	_cvsmhlls_rock_sml_01
		Geom	_cvsmhlls_rock_sml_02
		Geom	_cvsmhlls_rock_sml_03
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
		Bhvr	:RockOffset2.bhvr
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
		End
		Geom	Cav_rockSM_01
		Geom	Cav_rockSM_02
		Geom	Cav_rockSM_03
		Geom	_cvsmhlls_rock_sml_01
		Geom	_cvsmhlls_rock_sml_02
		Geom	_cvsmhlls_rock_sml_03
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
		Bhvr	:RockOffset3.bhvr
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
		End
		Geom	Cav_rockSM_01
		Geom	Cav_rockSM_02
		Geom	Cav_rockSM_03
		Geom	_cvsmhlls_rock_sml_01
		Geom	_cvsmhlls_rock_sml_02
		Geom	_cvsmhlls_rock_sml_03
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
		Bhvr	:RockOffset4.bhvr
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
		End
		Geom	Cav_rockSM_01
		Geom	Cav_rockSM_02
		Geom	Cav_rockSM_03
		Geom	_cvsmhlls_rock_sml_01
		Geom	_cvsmhlls_rock_sml_02
		Geom	_cvsmhlls_rock_sml_03
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
		Bhvr	:RockOffset5.bhvr
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
		End
		Geom	Cav_rockSM_01
		Geom	Cav_rockSM_02
		Geom	Cav_rockSM_03
		Geom	_cvsmhlls_rock_sml_01
		Geom	_cvsmhlls_rock_sml_02
		Geom	_cvsmhlls_rock_sml_03
		Lifespan 1120
		LifespanJitter 130
	End
End

#############################################################

End