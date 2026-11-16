#
FxInfo
Lifespan 300


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	Lava_ObeliskExplode_01 500 400 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			Type Start
			At Hips
			BhvrOverride
				PositionOffset 0 40 0
			End
			Part WORLD\Objects\DarkObelisk\DarkObelisk_Debris.part
			Part WORLD\Objects\DarkObelisk\DarkObeliskLightning_Explosion.part
			Part WORLD\Objects\DarkObelisk\DarkObeliskLightning_ExplosionAdd.part
			LifeSpan 20
			LifeSpanJitter 0
		End

	End

	Condition
		On Time
		Time 15

		Event
			Type Start
			At Hips
			BhvrOverride
				PositionOffset 0 85 0
			End
			Part WORLD\Objects\DarkObelisk\DarkObeliskLightning_Explosion.part
			Part WORLD\Objects\DarkObelisk\DarkObelisk_Debris.part
			Part WORLD\Objects\DarkObelisk\DarkObeliskLightning_ExplosionAdd.part
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 25

		Event
			Type Start
			At Skirt
			BhvrOverride
				PositionOffset 0 130 0
			End
			Part WORLD\Objects\DarkObelisk\DarkObeliskLightning_Explosion.part
			Part WORLD\Objects\DarkObelisk\DarkObelisk_Debris.part
			Part WORLD\Objects\DarkObelisk\DarkObeliskLightning_ExplosionAdd.part
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 60

		Event
			Type Start
			At Hips
			BhvrOverride
				PositionOffset 0 30 0
			End
			Part WORLD\Objects\DarkObelisk\DarkObeliskLightning_ExplosionGlowB.part
			Part WORLD\Objects\DarkObelisk\DarkObeliskLightning_ExplosionB.part
			Part WORLD\Objects\DarkObelisk\DarkObeliskLightning_ExplosionC.part
			LifeSpan 100
		End

	End

	Condition
		On Time
		Time 80

		Event
			Type Start
			At Hips
			BhvrOverride
				PositionOffset 0 50 0
			End
			Part WORLD\Objects\DarkObelisk\DarkObeliskLightning_ExplosionSmoke.part
			LifeSpan 200
		End

	End

	Condition
		On Time
		Time 55

		Event
			Type Start
			At Hips
			BhvrOverride
				PositionOffset 0 0 0
			End
			Part WORLD\Objects\DarkObelisk\DarkObeliskLightning_ExplosionGlowA.part
			LifeSpan 200
		End

	End

	Condition
		On Time
		Time 55

		Event
			Type Start
			At Skirt
			Bhvr behaviors\Geo_ObeliskPieces.bhvr
			BhvrOverride
				InitialVelocity 10 0 0
				Spin 0 0 0
				SpinJitter 0 50 0
				FadeOutLength 30
			End
			Geom Darkobelisk_PieceA
			LifeSpan 60
		End

	End

	Condition
		On Time
		Time 60

		Event
			Type Start
			At Skirt
			Bhvr behaviors\Geo_ObeliskPieces.bhvr
			BhvrOverride
				InitialVelocity 0 0 -10
				PositionOffset 0 50 0
				SpinJitter 0.40 0.100000 0
				FadeOutLength 30
			End
			Geom Darkobelisk_PieceB
			LifeSpan 60
		End

	End

	Condition
		On Time
		Time 62

		Event
			Type Start
			At ee
			Bhvr behaviors\Geo_ObeliskPieces.bhvr
			BhvrOverride
				InitialVelocity -8 5 8
				PositionOffset 0 100 0
				SpinJitter 0.30 0.100000 0
				FadeOutLength 30
			End
			Geom Darkobelisk_PieceC
			LifeSpan 60
		End

	End

	Condition
		On Time
		Time 62

		Event
			Type Start
			At SpadR
			Bhvr behaviors\Geo_ObeliskPieces.bhvr
			BhvrOverride
				InitialVelocity -7 0 -3
				PositionOffset 0 100 0
				SpinJitter 0.30 0.100000 0
				FadeOutLength 30
			End
			Geom Darkobelisk_PieceD
			LifeSpan 60
		End

	End

	Condition
		On Time
		Time 63

		Event
			Type Start
			At Emblem
			Bhvr behaviors\Geo_ObeliskPieces.bhvr
			BhvrOverride
				InitialVelocity 4 0 8
				PositionOffset 0 120 0
				SpinJitter 0.50 0.100000 0
				FadeOutLength 30
			End
			Geom Darkobelisk_PieceE
			LifeSpan 60
		End

	End

	Condition
		On Time
		Time 62

		Event
			Type Start
			At Emblem
			Bhvr behaviors\Geo_ObeliskPieces.bhvr
			BhvrOverride
				InitialVelocity 10 0 -10
				PositionOffset 0 120 0
				SpinJitter 0.50 0.100000 0
				FadeOutLength 30
			End
			Geom Darkobelisk_PieceF
			LifeSpan 60
		End

	End

	Condition
		On Time
		Time 62

		Event
			Type Start
			At SpadR
			Bhvr behaviors\Geo_ObeliskPieces.bhvr
			BhvrOverride
				InitialVelocity -10 0 10
				PositionOffset 0 100 0
				SpinJitter 0.50 0.100000 0
				FadeOutLength 30
			End
			Geom Darkobelisk_PieceG
			LifeSpan 60
		End

	End

	Condition
		On Time
		Time 60

		Event
			Type Start
			At Hips
			BhvrOverride
				PositionOffset 0 20 0
			End
			Part WORLD\Objects\DarkObelisk\DarkObelisk_DebrisC.part
			LifeSpan 100
		End

	End

End
