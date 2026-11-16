#
FxInfo

	LifeSpan 180


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BM_SummonDireWolf_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Head
			Type StartPositOnly
			EName UArmLFX
			POther Neck
			Part :Mist_Cold_Summon.part
			Part :Mist_Big.part
			Part :snowfall.part
			LifeSpan 140
		End

		Event
			At UArmL
			Type StartPositOnly
			EName UArmLFX
			POther LarmL
			Part :Mist_Cold_Summon.part
			Part Emotes\CostumeChanges\Ice\Mist_Big.part
			LifeSpan 140
		End

		Event
			At UArmR
			Type StartPositOnly
			EName UArmRFX
			POther LArmR
			Part :Mist_Cold_Summon.part
			Part :Mist_Big.part
			LifeSpan 140
		End

		Event
			At LArmL
			Type StartPositOnly
			EName LArmLFX
			POther HandL
			Part :Mist_Cold_Summon.part
			Part :Mist_Big.part
			Part :snowfall.part
			LifeSpan 140
		End

		Event
			At LArmR
			Type StartPositOnly
			EName LArmRFX
			POther HandR
			Part :Mist_Cold_Summon.part
			Part :Mist_Big.part
			Part :snowfall.part
			LifeSpan 140
		End

	End

Condition
	On 	Time
	Time	55
	Event
		Type	Local
		At	T_Root
		Sound 	BM_SummonDireWolf_Hit_01 100 100 .8
	End
End

	Condition
		On Time
		Time 55

		Event
			At T_Root
			Type Start
			BhvrOverride
				PositionOffset 0 0 0
				Scale 0.1 0.1 0.1
			End
			Part :TerribleHowl_Ring.part
			Part :TerribleHowl_Wind_Shockwave.part
			Part :TerribleHowl_Wind_Shockwave_Core.part
			LifeSpan 80
		End

		Event
			At T_Root
			Type Start
			Bhvr CustomizeablePowers\BeastMastery\Cold_Expanding.bhvr
			BhvrOverride
				PositionOffset 0 1 0
				Spin 0 -0.1 0
			End
			Part :TerribleHowl_Dust.part
			Part :Summon_Snow_Circle.part
			Part :TerribleHowl_Snow_Flakes.part
			LifeSpan 65
		End

		Event
			At T_Root
			Type Start
			BhvrOverride
				PositionOffset 0 1 0
			End
			Part :Summon_Snow_Circle2.part
			LifeSpan 65
		End

		Event
			At T_Root
			Type Start
			Bhvr CustomizeablePowers\BeastMastery\Cold_Expanding.bhvr
			BhvrOverride
				Scale 0.1 0.1 0.1
				tintgeom 1
			End
			Geom FX_Tintable_TerribleHowl
			LifeSpan 10
		End

		Event
			At T_Root
			Type Start
			Bhvr CustomizeablePowers\BeastMastery\Cold_Expanding.bhvr
			BhvrOverride
				PositionOffset 0 -1 0
				Scale 0.01 0.01 0.01
				tintgeom 1
			End
			Geom FX_Tintable_TerribleHowl
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 65

		Event
			At T_Root
			Type Start
			EName Rockup
			Bhvr :Geo_Ice_FadeIn.bhvr
			BhvrOverride
				PositionOffset 1 3 0
				TintGeom 1
			End
			Geom Glacier_Chunk_01
			Geom Glacier_Chunk_02
			LifeSpan 40
		End

		Event
			At T_Root
			Type Start
			EName Rockup
			Bhvr :Geo_Ice_FadeIn.bhvr
			BhvrOverride
				PositionOffset 1 3 1
				TintGeom 1
			End
			Geom Glacier_Chunk_01
			Geom Glacier_Chunk_02
			LifeSpan 40
		End

		Event
			At T_Root
			Type Start
			EName Rockup
			Bhvr :Geo_Ice_FadeIn.bhvr
			BhvrOverride
				PositionOffset -1 3 0
				TintGeom 1
			End
			Geom Glacier_Chunk_01
			Geom Glacier_Chunk_02
			LifeSpan 40
		End

		Event
			At T_Root
			Type Start
			EName Rockup
			Bhvr :Geo_Ice_FadeIn.bhvr
			BhvrOverride
				PositionOffset -1 3 -1
				TintGeom 1
			End
			Geom Glacier_Chunk_01
			Geom Glacier_Chunk_02
			LifeSpan 40
		End

	End

	Condition
		On Time
		Time 70

		Event
			At T_Root
			Type Start
			EName BodyFrost
			BhvrOverride
				PositionOffset 0 3 0
			End
			Part :IceGlow.part
		End

	End

	Condition
		On Time
		Time 102

		Event
			At T_Root
			Type Local
			EName Flash
			BhvrOverride
				PositionOffset 0 3 0
			End
			Part CustomizeablePowers\BeastMastery\IceBreakOutFlash.part
		End

		Event
			At T_Root
			Type Local
			BhvrOverride
				PositionOffset 0 0.2 0
			End
			Part CustomizeablePowers\BeastMastery\Summon_Snow_Residual.part
		End

	End

	Condition
		On Time
		Time 105
		Repeat 14

		Event
			At T_Root
			Type Start
			#CEvent WORLD\tunnels\CHILD_RockBreak.fx
			EName Kaboom
			Bhvr behaviors/FadeOut.bhvr
			BhvrOverride
				PositionOffset 0 3 0
				StartJitter 2 4 2
				Scale 0.1 0.15 0.1
				Spin 0.2 0.1 0.1
				InitialVelocity 0.0 1.2 0.0
				InitialVelocityJitter 1.5 1 1.5
				FadeOutLength 45
				Drag 0.0
				Gravity 0.2
				physics 1
				physGravity 4
				physRestitution 0.1
				physSFriction 5
				physDFriction 0.9
			End
			Part Emotes\CostumeChanges\Ice\IceBigScatter.part
			Geom Glacier_Chunk_03
			Geom Glacier_Chunk_04
			Geom Glacier_Chunk_05
			LifeSpan 90
		End

	End

	Condition
		On Time
		Time 105

		Event
			At T_Root
			Type Start
			BhvrOverride
				PositionOffset 0 3 0
			End
			Part Emotes\CostumeChanges\Ice\IceDust_Out.part
		End

	End

	Condition
		On Time
		Time 102

		Event
			At T_Root
			Type Local
			EName DirtAfter
			BhvrOverride
				PositionOffset 0 5 0
			End
			Part Emotes\CostumeChanges\Ice\Ice_LingeringScatter.part
			Part Emotes\CostumeChanges\Ice\Ice_LingeringFrost.part
			LifeSpan 30
		End

	End

End
