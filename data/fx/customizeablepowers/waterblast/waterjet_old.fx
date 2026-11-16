#
FxInfo

	LifeSpan 150


###########################################################

	Condition
		On Time
		Time 5

		Event
			At WepL
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :WaterShot_HandGlow.part
			Part :WaterShot_DrawIn_Spray.part
			LifeSpan 20
		End

		Event
			At WepR
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :WaterShot_HandGlow.part
			Part :WaterShot_DrawIn_Spray.part
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 25

		Event
			At Chest
			Type Start
			EName Prime
			LookAt T_Chest
			Magnet T_Chest
			Bhvr CustomizeablePowers\SonicAttack\SMLProjectile.bhvr
			BhvrOverride
				Spin 0 0 0.25
				TrackRate 3
			End
			#Geom Geo_Fx_TestSphere
		End

		Event
			Ename UpperAnchor
			At Prime
			Type Local
			#Bhvr Behaviors/GenericParticleFade.bhvr
			LookAt T_Chest
			Magnet T_Chest
			BhvrOverride
				#Scale 1 1 2
				PositionOffset 0 0 -4
				InitialVelocity 0 .1 0
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
			Part :WaterShot_Splash_Trail_05.part
			Part :WaterShot_Splash_Trail_06.part
			Part :WaterShot_Splash_Trail_07.part
		End

		Event
			Ename LowerAnchor
			At Prime
			Type Local
			#Bhvr Behaviors/GenericParticleFade.bhvr
			LookAt T_Chest
			Magnet T_Chest
			BhvrOverride
				#Scale 1 1 2
				PositionOffset 0 -0 -4
				InitialVelocity 0 -.1 0
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
			Part :WaterShot_Splash_Trail_05.part
			Part :WaterShot_Splash_Trail_06.part
			Part :WaterShot_Splash_Trail_07.part
		End

		Event
			At UpperAnchor
			Type Local
			EName LeftHandBall_Add
			Bhvr :BallSwirl_Grow.bhvr
			BhvrOverride
				Scale .25 .25 .25
				EndScale .3 .3 .8
				Spin -0.13 0.06 0
				Alpha 180
				TintGeom 1
			End
			Geom FX_WaterBall
		End

		Event
			At LowerAnchor
			Type Local
			EName LeftHandBall_Add
			Bhvr :BallSwirl_Grow.bhvr
			BhvrOverride
				Scale .25 .25 .25
				EndScale .3 .3 .8
				Spin -0.13 0.06 0
				Alpha 180
				TintGeom 1
			End
			Geom FX_WaterBall
		End

		Event
			At UpperAnchor
			Type Local
			EName LeftHandBall_Add
			Bhvr :BallSwirl_Grow.bhvr
			BhvrOverride
				Scale .25 .25 .25
				EndScale .3 .3 .8
				Spin -0.13 0.06 0
				Alpha 180
				TintGeom 1
			End
			Geom FX_WaterBall_Add
		End

		Event
			At LowerAnchor
			Type Local
			EName LeftHandBall_Add
			Bhvr :BallSwirl_Grow.bhvr
			BhvrOverride
				Scale .25 .25 .25
				EndScale .3 .3 .8
				Spin -0.13 0.06 0
				Alpha 180
				TintGeom 1
			End
			Geom FX_WaterBall_Add
		End

		Event
			At UpperAnchor
			Type Local
			BhvrOverride
				PositionOffset 0 0 -4
				PyrRotate 0 180 0
				Alpha 120
				Scale .3 .3 1.2
			End
			Geom FX_WaterSpray
		End

		Event
			At LowerAnchor
			Type Local
			BhvrOverride
				PositionOffset 0 0 -4
				PyrRotate 0 180 0
				Alpha 120
				Scale .3 .3 1.2
			End
			Geom FX_WaterSpray
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
