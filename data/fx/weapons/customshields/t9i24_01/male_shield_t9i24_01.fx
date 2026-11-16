#############################################################
## Costume Anim Entity - Shield - T9I24_01
#############################################################

FxInfo

Flags InheritAlpha DontSuppress IsShield

#############################################################

Condition
	On Time
	Time 	0
	// TriggerbitS SHIELD
	// Domany 1

	Event
		// Ename	ShieldFX
		Type	Local
		At	LArmL
			Bhvr	:Male_Shield_T9I24_01.bhvr
			BhvrOverride
				TintGeom 1
			End
			Geom	FX_Shield_T9i24_Base_01
		// while	SHIELD
		// Flags	OneAtATime
		// ChildFX :Male_Shield_T9I24_01_CHILD.fx
	End

	Event
		Type	Local
		At	LArmL
			Bhvr	:Male_Shield_T9I24_01.bhvr
			BhvrOverride
				TintGeom 1
			End
			Geom	FX_Shield_T9i24_Detail_01
	End

	Event
		Type	Local
		At	LArmL
			Bhvr	:Male_Shield_T9I24_01.bhvr
			BhvrOverride
				TintGeom 1
			End
			Geom	FX_Shield_T9i24_Detail_02
	End

	Event
		Ename ShieldFX
		Type	Local
		At	LArmL
			Bhvr	:Male_Shield_T9I24_01.bhvr
			BhvrOverride
				TintGeom 1
			End
			Geom	FX_Shield_T9i24_ChargeFX
	End

	Event
		Type Local
		At ShieldFX
		AltPiv 1
		Part WEAPONS\CustomShields\T9I24_01\Shield_Rim_Glowies.part
	End


	Event
		Type Local
		At ShieldFX
		AltPiv 2
		Part WEAPONS\CustomShields\T9I24_01\Shield_Rim_Glowies.part
	End


	Event
		Type Local
		At ShieldFX
		AltPiv 3
		Part WEAPONS\CustomShields\T9I24_01\Shield_Rim_Glowies.part
	End


	Event
		Type Local
		At ShieldFX
		AltPiv 4
		Part WEAPONS\CustomShields\T9I24_01\Shield_Rim_Glowies.part
	End


	Event
		Type Local
		At ShieldFX
		AltPiv 5
		Part WEAPONS\CustomShields\T9I24_01\Shield_Rim_Glowies.part
	End


	Event
		Type Local
		At ShieldFX
		AltPiv 6
		Part WEAPONS\CustomShields\T9I24_01\Shield_Rim_Glowies.part
	End


	Event
		Type Local
		At ShieldFX
		AltPiv 7
		Part WEAPONS\CustomShields\T9I24_01\Shield_Rim_Glowies.part
	End


	Event
		Type Local
		At ShieldFX
		AltPiv 8
		Part WEAPONS\CustomShields\T9I24_01\Shield_Rim_Glowies.part
	End

	Event
		Type Local
		At ShieldFX
		BhvrOverride
			PositionOffset  0,  0,  -0.07
		End

		AltPiv 9
		Part WEAPONS\CustomShields\T9I24_01\Shield_Rim_Glow.part
		Part WEAPONS\CustomShields\T9I24_01\Shield_Lower_Glow.part
	End


	Event
		Type Local
		At ShieldFX
		BhvrOverride
			PyrRotate  0,  210,  0
			PositionOffset  0,  0,  -0.4
		End

		AltPiv 9
		Part WEAPONS\CustomShields\T9I24_01\Shield_Upper_Arcs.part
	End

End
#############################################################

End