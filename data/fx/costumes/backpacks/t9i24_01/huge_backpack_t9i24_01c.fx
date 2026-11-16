# T9I24_01 Backpack B with Plugs
FxInfo

	Flags InheritAlpha DontSuppress

###########################################################

	Condition
		On Time
		Time 0

		Event
			Anim FX_Backpack_T9I24_01B
			At Back
			Type Local
			BhvrOverride
				PositionOffset 0 -0.1 -0.33
				PyrRotate -5 0 0
				Scale 1.3 1.3 1.3
			End
		End
	End

	Condition
		On Time
		Time 0

		Event
			Anim FX_Backpack_T9I24_01C
			At Back
			Type Local
			BhvrOverride
				PositionOffset 0 -0.4 -0.7
				PyrRotate -45 0 0
				Scale 1.2 1.2 1.2
			End
		End
	End

	Condition
		On Time
		Time 0

		Event
			Anim FX_Backpack_T9I24_01C
			At Back
			Type Local
			BhvrOverride
				PositionOffset 0.2 -0.5 -0.5
				PyrRotate -35 -30 0
				Scale 1.2 1.2 1.2
			End
		End
	End

	Condition
		On Time
		Time 0

		Event
			Anim FX_Backpack_T9I24_01C
			At Back
			Type Local
			BhvrOverride
				PositionOffset -0.2 -0.5 -0.5
				PyrRotate -35 30 0
				Scale 1.2 1.2 1.2
			End
		End
	End

	Condition
		On Time
		Time 0

		Event
			Anim FX_Backpack_T9I24_01D
			At Back
			Type Local
			BhvrOverride
				PositionOffset 0 -0.4 -0.7
				PyrRotate -45 0 0
				Scale 1.2 1.2 1.2
			End
		End
	End

	Condition
		On Time
		Time 0

		Event
			Anim FX_Backpack_T9I24_01D
			At Back
			Type Local
			BhvrOverride
				PositionOffset 0.2 -0.5 -0.5
				PyrRotate -35 -30 0
				Scale 1.2 1.2 1.2
			End
		End
	End

	Condition
		On Time
		Time 0

		Event
			Anim FX_Backpack_T9I24_01D
			At Back
			Type Local
			BhvrOverride
				PositionOffset -0.2 -0.5 -0.5
				PyrRotate -35 30 0
				Scale 1.2 1.2 1.2
			End
		End

##############

		// Event
		// 	Geom Backpack_T9I24_FX
		// 	At Back
		// 	Type Local
		// 	BhvrOverride
		// 		Tintgeom 1
		// 		PositionOffset 0 -0.4 -0.7
		// 		PyrRotate -45 0 0
		// 		Scale 1.2 1.2 1.2
		// 	End
		// End

		// Event
		// 	Geom Backpack_T9I24_FX_Glow
		// 	At Back
		// 	Type Local
		// 	BhvrOverride
		// 		Tintgeom 1
		// 		Alpha	180
		// 		PositionOffset 0 -0.4 -0.7
		// 		PyrRotate -45 0 0
		// 		Scale 1.2 1.2 1.2
		// 	End
		// End

		// Event
		// 	Geom Backpack_T9I24_FX
		// 	At Back
		// 	Type Local
		// 	BhvrOverride
		// 		Tintgeom 1
		// 		PositionOffset 0.2 -0.5 -0.5
		// 		PyrRotate -35 -30 0
		// 		Scale 1.2 1.2 1.2
		// 	End
		// End

		// Event
		// 	Geom Backpack_T9I24_FX_Glow
		// 	At Back
		// 	Type Local
		// 	BhvrOverride
		// 		Tintgeom 1
		// 		Alpha	180
		// 		PositionOffset 0.2 -0.5 -0.5
		// 		PyrRotate -35 -30 0
		// 		Scale 1.2 1.2 1.2
		// 	End
		// End

		// Event
		// 	Geom Backpack_T9I24_FX
		// 	At Back
		// 	Type Local
		// 	BhvrOverride
		// 		Tintgeom 1
		// 		PositionOffset -0.2 -0.5 -0.5
		// 		PyrRotate -35 30 0
		// 		Scale 1.2 1.2 1.2
		// 	End
		// End

		// Event
		// 	Geom Backpack_T9I24_FX_GLow
		// 	At Back
		// 	Type Local
		// 	BhvrOverride
		// 		Tintgeom 1
		// 		Alpha	180
		// 		PositionOffset -0.2 -0.5 -0.5
		// 		PyrRotate -35 30 0
		// 		Scale 1.2 1.2 1.2
		// 	End
		// End

##########

		// Event
		// 	Geom Backpack_T9i24_FX_Offsets
		// 	Ename GenAnchor
		// 	At Back
		// 	Type Local
		// 	BhvrOverride
		// 		PositionOffset 0 -0.1 -0.37
		// 		PyrRotate -5 0 0
		// 		Scale 1.27 1.27 1.27
		// 		Alpha 1
		// 	End
		// End

		// Event
		// 	At GenAnchor
		// 	Type Local
		// 	Altpiv	9
		// 	Part	:Huge_Center_PlasmaSphere.part
		// 	Part	:Huge_Center_PlasmaSphere_Additive.part
		// 	Part	:Huge_Center_Ring.part
		// 	Part	:Huge_Center_Electricity.part
		// End

		// Event
		// 	Ename	BeamAchor1
		// 	At GenAnchor
		// 	Type Local
		// 	AltPiv	1
		// 	Part	:Huge_Rim_Glowies.part
		// 	Part	:Huge_Node_ElectricBolt.part
		// End

		// Event
		// 	Ename	BeamAchor2
		// 	At GenAnchor
		// 	Type Local
		// 	AltPiv	2
		// 	BhvrOverride
		// 		PyrRotate 0 0 45
		// 	End
		// 	Part	:Huge_Rim_Glowies.part
		// 	Part	:Huge_Node_ElectricBolt.part
		// End

		// Event
		// 	Ename	BeamAchor3
		// 	At GenAnchor
		// 	Type Local
		// 	AltPiv	3
		// 	BhvrOverride
		// 		PyrRotate 0 0 90
		// 	End
		// 	Part	:Huge_Rim_Glowies.part
		// 	Part	:Huge_Node_ElectricBolt.part
		// End

		// Event
		// 	Ename	BeamAchor4
		// 	At GenAnchor
		// 	Type Local
		// 	AltPiv	4
		// 	BhvrOverride
		// 		PyrRotate 0 0 135
		// 	End
		// 	Part	:Huge_Rim_Glowies.part
		// 	Part	:Huge_Node_ElectricBolt.part
		// End

		// Event
		// 	At GenAnchor
		// 	Type Local
		// 	AltPiv	5
		// 	BhvrOverride
		// 		PyrRotate 0 0 180
		// 	End
		// 	Part	:Huge_Rim_Glowies.part
		// 	Part	:Huge_Node_ElectricBolt.part
		// End

		// Event
		// 	At GenAnchor
		// 	Type Local
		// 	AltPiv	6
		// 	BhvrOverride
		// 		PyrRotate 0 0 225
		// 	End
		// 	Part	:Huge_Rim_Glowies.part
		// 	Part	:Huge_Node_ElectricBolt.part
		// End

		// Event
		// 	At GenAnchor
		// 	Type Local
		// 	AltPiv	7
		// 	BhvrOverride
		// 		PyrRotate 0 0 270
		// 	End
		// 	Part	:Huge_Rim_Glowies.part
		// 	Part	:Huge_Node_ElectricBolt.part
		// End

		// Event
		// 	At GenAnchor
		// 	Type Local
		// 	AltPiv	8
		// 	BhvrOverride
		// 		PyrRotate 0 0 315
		// 	End
		// 	Part	:Huge_Rim_Glowies.part
		// 	Part	:Huge_Node_ElectricBolt.part
		// End
	End
End
