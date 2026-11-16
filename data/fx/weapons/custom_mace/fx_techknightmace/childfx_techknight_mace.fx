#############################################################
## Costume Anim Entity - Tech Knight Mace
#############################################################

FxInfo

Flags InheritAlpha InheritAnimScale DontSuppress

#############################################################
Condition

	On 	Time
	Time 	0

	Event
		EName	FXAnchor
		Type	Local
		At		WepR
		Geom	FX_TechKnightMace_FXAnchor01
	End

	Event
		EName	FXAnchor02
		Type	Local
		At		WepR
		Geom	FX_TechKnightMace_FXAnchor02
	End


#############################################################

	Event
		EName	FXAnchor_Center
		Type	Local
		At	FXAnchor
		AltPiv	13
		bhvr	behaviors\cagespin2.bhvr
		BhvrOverride
			Scale	   0.05 0.05 0.05
			TintGeom	1
		End
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Center_PlasmaSphere.part
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Center_PlasmaSphere_Additive.part
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Center_Ring.part
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Center_Electricity.part
		// Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_Glow_Base.part

	End

#############################################################

	Event
		// EName	Bolt1
		Type	Local
		At	FXAnchor
		AltPiv	1
		BhvrOverride
				Scale 0.1 0.1 0.1
				TintGeom 		1
		End
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_Glow.part
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_ElectricBolt.part
		POther FXAnchor_Center
		PMagnet FXAnchor_Center
	End

	Event
		// EName	Bolt2
		Type	Local
		At	FXAnchor
		AltPiv	2
		BhvrOverride
				Scale 0.1 0.1 0.1
				TintGeom 		1
		End
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_Glow.part
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_ElectricBolt.part
		POther FXAnchor_Center
		PMagnet FXAnchor_Center

	End

	Event
		// EName	Bolt3
		Type	Local
		At	FXAnchor
		AltPiv	3
		BhvrOverride
				Scale 0.1 0.1 0.1
				TintGeom 		1
		End
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_Glow.part
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_ElectricBolt.part
		POther FXAnchor_Center
		PMagnet FXAnchor_Center

	End

	Event
		// EName	Bolt4
		Type	Local
		At	FXAnchor
		AltPiv	4
		BhvrOverride
				Scale 0.1 0.1 0.1
				TintGeom 		1
		End
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_Glow.part
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_ElectricBolt.part
		POther FXAnchor_Center
		PMagnet FXAnchor_Center
	End

	Event
		// EName	Bolt5
		Type	Local
		At	FXAnchor
		AltPiv	5
		BhvrOverride
				Scale 0.1 0.1 0.1
				TintGeom 		1
		End
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_Glow.part
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_ElectricBolt.part
		POther FXAnchor_Center
		PMagnet FXAnchor_Center

	End

	Event
		// EName	Bolt6
		Type	Local
		At	FXAnchor
		AltPiv	6
		BhvrOverride
				Scale 0.1 0.1 0.1
				TintGeom 		1
		End
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_Glow.part
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_ElectricBolt.part
		POther FXAnchor_Center
		PMagnet FXAnchor_Center

	End

	Event
		// EName	Bolt7
		Type	Local
		At	FXAnchor
		AltPiv	7
		BhvrOverride
				Scale 0.1 0.1 0.1
				TintGeom 		1
		End
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_Glow.part
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_ElectricBolt.part
		POther FXAnchor_Center
		PMagnet FXAnchor_Center

	End

	Event
		// EName	Bolt8
		Type	Local
		At	FXAnchor
		AltPiv	8
		BhvrOverride
				Scale 0.1 0.1 0.1
				TintGeom 		1
		End
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_Glow.part
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_ElectricBolt.part
		POther FXAnchor_Center
		PMagnet FXAnchor_Center

	End

	Event
		// EName	Bolt9
		Type	Local
		At	FXAnchor
		AltPiv	9
		BhvrOverride
				Scale 0.1 0.1 0.1
				TintGeom 		1
		End
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_Glow.part
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_ElectricBolt.part
		POther FXAnchor_Center
		PMagnet FXAnchor_Center

	End

	Event
		// EName	Bolt10
		Type	Local
		At	FXAnchor
		AltPiv	10
		BhvrOverride
				Scale 0.1 0.1 0.1
				TintGeom 		1
		End
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_Glow.part
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_ElectricBolt.part
		POther FXAnchor_Center
		PMagnet FXAnchor_Center

	End

	Event
		// EName	Bolt11
		Type	Local
		At	FXAnchor
		AltPiv	11
		BhvrOverride
				Scale 0.1 0.1 0.1
				TintGeom 	1
		End
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_Glow.part
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_ElectricBolt.part
		POther FXAnchor_Center
		PMagnet FXAnchor_Center

	End

	Event
		// EName	Bolt12
		// EName	TestEvent
		Type	Local
		At	FXAnchor
		AltPiv	12
		BhvrOverride
				Scale 0.1 0.1 0.1
				TintGeom 	1
		End
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_Glow.part
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_ElectricBolt.part
		POther 	FXAnchor_Center
		PMagnet FXAnchor_Center

	End

	Event
		// EName	Bolt13
		Type	Local
		At	FXAnchor
		AltPiv	13
		BhvrOverride
				Scale 0.1 0.1 0.1
				TintGeom 		1
		End
		POther FXAnchor_Center

	End

	Event
		// EName	Glow1
		Type	Local
		At	FXAnchor02
		AltPiv	1
		BhvrOverride
				Scale 0.1 0.1 0.1
				TintGeom 		1
		End
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_Glow.part
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_ElectricBolt.part
		POther FXAnchor_Center
		PMagnet FXAnchor_Center
	End

	Event
		// EName	Glow2
		Type	Local
		At	FXAnchor02
		AltPiv	2
		BhvrOverride
				Scale 0.1 0.1 0.1
				TintGeom 		1
		End
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_Glow.part
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_ElectricBolt.part
		POther FXAnchor_Center
		PMagnet FXAnchor_Center
	End

	Event
		// EName	Glow3
		Type	Local
		At	FXAnchor02
		AltPiv	3
		BhvrOverride
				Scale 0.1 0.1 0.1
				TintGeom 		1
		End
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_Glow.part
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_ElectricBolt.part
		POther FXAnchor_Center
		PMagnet FXAnchor_Center
	End

	Event
		// EName	Glow4
		Type	Local
		At	FXAnchor02
		AltPiv	4
		BhvrOverride
				Scale 0.1 0.1 0.1
				TintGeom 		1
		End
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_Glow.part
		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_ElectricBolt.part
		POther FXAnchor_Center
		PMagnet FXAnchor_Center
	End

	Event
		// EName	Glow6
		EName	FXAnchor_Top
		Type	Local
		At	FXAnchor02
		AltPiv	6
		BhvrOverride
				PositionOFfset	0 0 0.0
				Scale 0.1 0.1 0.1
				TintGeom 		1
		End
   	End

	// Event
	// 	EName	Glow5
	// 	Type	Local
	// 	At	FXAnchor02
	// 	AltPiv	5
	// 	BhvrOverride
	// 			PositionOFfset	0 0 0.4
	// 			Scale 0.1 0.1 0.1
	// 			TintGeom 		1
	// 	End
    // 
	// 	POther 	FXAnchor_Top
	// End

	Event
		// EName	GlowTop
		Type	Local
		At	FXAnchor02
		AltPiv	5
		BhvrOverride
				Scale 0.1 0.1 0.1
				TintGeom 		1
		End
  		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_Glow_Top.part
    		Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_Glow_Base.part

		// Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_ElectricBolt_Top.part
		// Part	WEAPONS\Custom_Mace\FX_TechKnightMace\Node_ElectricBolt_Top_Core.part

		POther 	FXAnchor_Top
		// PMagnet FXAnchor_Top
	End


End

#############################################################

End
