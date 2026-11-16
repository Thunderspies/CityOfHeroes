#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At WepR
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :WaterShot_HandGlow.part
			Part :WaterShot_DrawIn_Spray.part
			Part :WaterBall_DrawIn_Spray2.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 15

		Event
			At WepR
			Type Start
			EName Prime
			LookAt T_Chest
			Magnet T_Chest
			Bhvr CustomizeablePowers\SonicAttack\SMLProjectile.bhvr
			BhvrOverride
				InitialVelocity 0 0.1 0
				TrackRate 3
			End
			Part :WaterShot_Splash_Initial01.part
			Part :WaterShot_Splash_Initial02.part
			Part :WaterShot_Splash_Trail_01.part
			Part :WaterShot_Splash_Trail_02.part
			Part :WaterShot_Splash_Trail_03.part
			Part :WaterShot_Splash_Trail_04.part
			Part :WaterShot_Bullet_SM.part
			#Part :WaterShot_Drips.part
			Part :WaterShot_Drips2.part
		End

#		Event
#			At Prime
#			Type Local
#
#			***** ADD PROJECTILE SOUND HERE *****
#
#		End

	End

	Condition
		On PrimeHit

		Event
			Type Destroy
			EName Prime
		End

		Event
			At T_Chest
			Type Local
			#ChildFx :Judgment_Pyronic_Hit.fx
		End

	End

End
