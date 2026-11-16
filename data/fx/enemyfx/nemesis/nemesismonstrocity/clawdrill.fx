#
FxInfo

	LifeSpan 2000


###########################################################
Condition
	On 	Time
	Time	10
	Event
		Type	Local
		At	Root
		Sound 	ClawDrill_01 100 100 .8
	End
End

	Condition
		On Time
		Time 48

		Event
			At Root
			Type Local
			EName DrillAnchor
			BhvrOverride
				PositionOffset 0 0 3.6
			End
			#Geom GEO_FX_testsphere
		End

		Event
			At DrillAnchor
			Type StartPositOnly
			Bhvr behaviors/CameraShake01.bhvr
			BhvrOverride
				PositionOffset 0 -1 0
				ShakeRadius 120
			End
			Part ENEMYFX\Praetorian\Warworks\Dust_Circle_Large.part
			Part ENEMYFX\Praetorian\Warworks\Dust_Circle_Lingering.part
			Part ENEMYFX\Praetorian\Warworks\Dust_Shockwave_Large.part
			Part ENEMYFX\Praetorian\Warworks\Dust_Shockwave_Fill.part
			Part ENEMYFX\Praetorian\Warworks\Dust_Flash.part
			Part ENEMYFX\Praetorian\Warworks\Sparks_Flash.part
			LifeSpan 8
		End

		Event
			At DrillAnchor
			Type Start
			Bhvr behaviors\Splat_Cracks.bhvr
			BhvrOverride
				Alpha 255
				PositionOffset 0 0.05 0
				Scale 50 50 50
				PyrRotate 0 180 0
			End
			Splat Crater_Cracks2.tga
			LifeSpan 150
		End

		Event
			At DrillAnchor
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0.0 -3.0 0.0
			End
			Part ENEMYFX\Praetorian\Warworks\Sparks_Vertical.part
			Part ENEMYFX\Praetorian\Warworks\Sparks_Long.part
			Part ENEMYFX\Praetorian\Warworks\Rocks.part
			Part :Rocks_Big.part
			Part :DrillDust_Circle_Large.part
			LifeSpan 16
		End

		Event
			At DrillAnchor
			Type StartPositOnly
			POther DustAnchor
			BhvrOverride
				PositionOffset 0.0 8.0 0.0
			End
			Part ENEMYFX\Praetorian\Warworks\Dust_Pillar.part
			Part ENEMYFX\Praetorian\Warworks\Dirt_Pillar.part
			LifeSpan 5
		End

	End

	Condition
		On Time
		Time 58

		Event
			At DrillAnchor
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0.0 -3.0 0.0
			End
			Part ENEMYFX\Praetorian\Warworks\Rocks.part
			Part :Rocks_Big.part
			Part :DrillDust_Circle_Large.part
			LifeSpan 16
		End

	End

	Condition
		On Time
		Time 68

		Event
			At DrillAnchor
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0.0 -3.0 0.0
			End
			Part ENEMYFX\Praetorian\Warworks\Rocks.part
			Part :DrillDust_Circle_Large.part
			LifeSpan 16
		End

	End

	Condition
		On Time
		Time 46
		Repeat 8

		Event
			At DrillAnchor
			Type StartPositOnly
			CEvent Emotes\CostumeChanges\Stone\CHILD_RockBreak.fx
			EName RockGrind
			Bhvr behaviors/FadeOut.bhvr
			BhvrOverride
				StartJitter 2 4 2
				Scale 0.2 0.3 0.2
				Spin 0.2 0.1 0.1
				InitialVelocity 0.0 0.7 0.0
				InitialVelocityJitter 1.5 0.5 1.5
				FadeOutLength 45
				Drag 0.0
				physics 1
				physGravity 1
				physRestitution 0.1
				physSFriction 5
				physDFriction 0.9
			End
			Geom Cliff_Chunk_03
			Geom Cliff_Chunk_04
			Geom Cliff_Chunk_05
			LifeSpan 90
		End

	End

	Condition
		On Time
		Time 56
		Repeat 8

		Event
			At DrillAnchor
			Type StartPositOnly
			CEvent Emotes\CostumeChanges\Stone\CHILD_RockBreak.fx
			EName Kaboom
			Bhvr behaviors/FadeOut.bhvr
			BhvrOverride
				StartJitter 2 4 2
				Scale 0.2 0.3 0.2
				Spin 0.2 0.1 0.1
				InitialVelocity 0.0 0.7 0.0
				InitialVelocityJitter 1.5 0.5 1.5
				FadeOutLength 45
				Drag 0.0
				physics 1
				physGravity 1
				physRestitution 0.1
				physSFriction 5
				physDFriction 0.9
			End
			Geom Cliff_Chunk_03
			Geom Cliff_Chunk_04
			Geom Cliff_Chunk_05
			LifeSpan 90
		End

	End

	Condition
		On Time
		Time 66
		Repeat 8

		Event
			At DrillAnchor
			Type StartPositOnly
			CEvent Emotes\CostumeChanges\Stone\CHILD_RockBreak.fx
			EName Kaboom
			Bhvr behaviors/FadeOut.bhvr
			BhvrOverride
				StartJitter 2 4 2
				Scale 0.2 0.3 0.2
				Spin 0.2 0.1 0.1
				InitialVelocity 0.0 0.7 0.0
				InitialVelocityJitter 1.5 0.5 1.5
				FadeOutLength 45
				Drag 0.0
				physics 1
				physGravity 1
				physRestitution 0.1
				physSFriction 5
				physDFriction 0.9
			End
			Geom Cliff_Chunk_03
			Geom Cliff_Chunk_04
			Geom Cliff_Chunk_05
			LifeSpan 90
		End

	End

End
