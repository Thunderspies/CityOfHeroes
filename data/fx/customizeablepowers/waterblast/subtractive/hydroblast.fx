#
FxInfo

	LifeSpan 150

############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	WB_HydroBlast 100 100 .8
	End
End


###########################################################

	Condition
		On Time
		Time 8

		Event
			EName mainAnchor
			At Chest
			Type Local
			BhvrOverride
				PositionOffset 0 2 1
			End
		End

		Event
			EName BallAnchor
			At mainAnchor
			Type Local
			BhvrOverride
				Spin 0 0.1 0
			End
		End

		Event
			EName DripAnchor
			At mainAnchor
			Type PositOnly
		End

		Event
			EName InitSplashAnchor
			At Root
			Type Local
			BhvrOverride
				PositionOffset 1 0 0.5
			End
		End

		Event
			Ename WaterSwirl
			At DripAnchor
			Type Local
			Bhvr :Swirl_Grow.bhvr
			BhvrOverride
				PositionOffset 0 -0.75 0
				Tintgeom 1
			End
			Geom FX_Tintable_TerribleHowl
			LifeSpan 25
		End

		Event
			Ename	DrawIn
			At DripAnchor
			Type Start
			Bhvr :Swirl_Grow.bhvr
			BhvrOverride
				PositionOffset 0 -0.5 0
			End
			Part :WaterBall_DrawIn_Spray.part
			Part :WaterBall_DrawIn_Spray2.part
			LifeSpan 8
		End

		Event
			Ename	SplashIn
			At DripAnchor
			Type Local
			Bhvr :Swirl_Grow.bhvr
			BhvrOverride
				PositionOffset 0 -0.5 0
			End
			Part :WaterBall_DrawIn_Splatter.part
			LifeSpan 6
		End

		Event
			Ename TheDrips
			At DripAnchor
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 -0.3 0
				StartJitter .6 .5 .6
			End
			Part :Drips.part
			LifeSpan 5
		End

		Event
			Ename GroundSplashes
			At InitSplashAnchor
			Type Local
			BhvrOverride
				StartJitter 1 0 1
			End
			Part :RainDrop_Splash_Small.part
			LifeSpan 1
		End

	End

	Condition
		On Time
		Time 12

		Event
			Ename StreaksIn
			At BallAnchor
			Type Local
			EName LeftHandBallGlow
			Bhvr :BallSwirl_Grow.bhvr
			BhvrOverride
				#PositionOffset 0.2 0 0
			End
			Part :WaterShot_GlowStreaks.part
			#Part :WaterShot_CircleMist.part
			#Part :WaterShot_Glow.part
			LifeSpan 15
		End

	End

	Condition
		On Time
		Time 14

		Event
			Ename GeoBall
			At BallAnchor
			Type Local
			EName LeftHandBall
			Bhvr :BallSwirl_Grow.bhvr
			BhvrOverride
				PositionOffset 0.2 0 0
				Spin -0.15 0.06 0
				Alpha 255
				TintGeom 1
			End
			Geom FX_WaterBall
			LifeSpan 20
		End

		Event
			Ename GeoBall2
			At BallAnchor
			Type Local
			EName LeftHandBall_Add
			Bhvr :BallSwirl_Grow.bhvr
			BhvrOverride
				PositionOffset 0.2 0 0
				Spin -0.15 0.0 0
				Alpha 255
				TintGeom 1
			End
			Geom FX_WaterBall
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 32

		Event
			At mainAnchor
			Type Start
			EName Prime
			LookAt Target
			Magnet Target
			Bhvr CustomizeablePowers\SonicAttack\SMLProjectile.bhvr
			BhvrOverride
				InitialVelocity 0 0.6 0
				Scale 1.2 1 1.2
				TrackRate 2.5
			End
			Part :WaterBall_Drips.part
		End

		Event
			At Prime
			Type Local
			EName LeftHandBall
			Bhvr :BallSwirl_Grow.bhvr
			BhvrOverride
				Scale .25 .25 .25
				EndScale .3 .3 .8
				Spin -0.13 0.06 0
				Alpha 255
				TintGeom 1
			End
			Geom FX_WaterBall
		End

		Event
			At Prime
			Type Local
			EName LeftHandBall_Add
			Bhvr :BallSwirl_Grow.bhvr
			BhvrOverride
				Scale .25 .25 .25
				EndScale .3 .3 .8
				Spin -0.13 0.06 0
				Alpha 255
				TintGeom 1
			End
			Geom FX_WaterBall
		End

		Event
			At Prime
			Type Local
			EName LeftHandBall_Drips
			#Part :WaterBall_Drips_Local.part
			Part :WaterShot_Splash_Initial01.part
			Part :WaterShot_Splash_Initial02.part
			Part :WaterShot_Splash_Trail_01.part
			Part :WaterShot_Splash_Trail_05.part
			Part :WaterShot_Splash_Trail_06.part
			Part :WaterShot_Splash_Trail_07.part
			Part :WaterShot_Drips2.part
		End

	End

	Condition
		On PrimeHit

		Event
			Type Destroy
			EName Prime
		End

	End

End
