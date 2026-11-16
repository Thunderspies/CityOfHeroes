#
FxInfo

	Flags InheritAnimScale

	LifeSpan 20


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
				PyrRotateJitter 60 180 30
			End
			#LifeSpan 5
		End

		Event
			At SpineRotation
			Type Local
			Bhvr :Quills_Projectile.bhvr
			BhvrOverride
				InitialVelocity 0 0 4
				Scale -1 -1 -2
				#Scale 0.03 0.03 0.4
				Spin 0 0 0
				FadeOutLength 5
				StartColor 1 1 1
			End
			#Part Events\EndGame\Magisterium\Shard_Trail_blobs.part
			#Part :Shard_Trail.part
			Geom ObsidianShard
			LifeSpan 5
		End

	End

End
