#
FxInfo

	Flags InheritAnimScale

	LifeSpan 30


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Hips
			Type Start
			EName SpineRotation
			BhvrOverride
				PyrRotate 45 0 0
				PyrRotateJitter 75 180 0
			End
			LifeSpan 20
		End

		Event
			At SpineRotation
			Type Local
			Bhvr ENEMYFX\BanishedPantheon\BanishedPantheon_Revamped\FrigidQuills_Projectile.bhvr
			BhvrOverride
				InitialVelocity 0 0 6
				Scale 0.03 0.03 0.4
				Spin 0 0 0
				FadeOutLength 5
				StartColor 255 0 0
			End
			Part :StoneShard_Trail.part
			Geom Cliff_Chunk_05
			Geom Cliff_Chunk_04
			LifeSpan 13
		End

	End

End
