#############################################################
## AntimatterPulse.fx
#############################################################

FxInfo
Lifespan 1200


#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	start
		At	Root
		BhvrOverride
			PositionOffset	0.0 0.5 0.0
		End
		Part	:Target_Reticle_Small.part
		Part	:Target_Fill.part
		Lifespan 1
	End


End

Condition
	On	Time
	Time	15

	Event
		Type	start
		At	Root
		BhvrOverride
			PositionOffset	0.0 0.5 0.0
		End
		Part	:DustCircle.part
		Part	:Dust_Rising.part
	Sound Orbital_Beam_01 200 200 .9
		Lifespan 335
	End
End

Condition
	On	Time
	Time	30

	Event
		EName	RockAnchor
		Type	start
		At	Root
		Bhvr	:CameraShake_Rumble.bhvr
		Lifespan 250
	End

	Event
		Type	start
		At	Root
		Part	:Pebbles_Rising.part
		Lifespan 250
	End
End

Condition
	On	Time
	Time	60
	Repeat 20

	Event
		Type	start
		At	Root
		Bhvr	CustomizeablePowers\GravityControl\GCPropelScale.bhvr
		BhvrOverride
			StartColor		255 240 200
			Gravity			-0.1
			Drag			.8
			StartJitter		40 2.5 40
			PyrRotateJitter		180 180 180
			SpinJitter		0.15 0.15 0.15
			physGravity		1.5
			physRestitution 	0.5
			physSFriction 	 	0.99
			physDFriction 	 	0.9
		End

		Geom	Earth_FXDebris01A
		Geom	Earth_FXDebris02A
		Geom	Earth_FXDebris03A
		Geom	Earth_FXDebris04A
		Geom	Earth_FXDebris05A

		geom	GEO_Slag_Golem_Junk_19
		geom	GEO_Slag_Golem_Junk_35
		geom	GEO_Slag_Golem_Junk_39
		geom	GEO_Slag_Golem_Junk_40
		geom	FX_P_Bottle

		Lifespan 220
		CThresh 	0.00001
	End
End

Condition
	On	Cycle
	Time	10
	Repeat	3

	Event
		Type	start
		At	RockAnchor
		Bhvr	CustomizeablePowers\GravityControl\GCPropelScale.bhvr
		BhvrOverride
			Scale			0.5 0.5 0.5
			StartColor		255 240 200
			Gravity			-0.8
			Drag			.8
			PositionOffset		0.0 -10.0 0.0
			StartJitter		40 0.0 40
			SpinJitter		0.25 0.25 0.25
			physGravity		1.5
			physRestitution 	0.5
			physSFriction 	 	0.99
			physDFriction 	 	0.9
		End

		Geom	Geo_leaf_maple
		Geom	Geo_leaf_oak

		Lifespan 250
		CThresh 	0.00001
	End


End

Condition
	On 	Time
	Time 	150

	Event
		EName	RockAnchor2
		Type	start
		At	Root
		ChildFX	:OrbitalBeam_Attack.fx
		Lifespan 300
	End

End

#############################################################

End
