#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Start
			EName BoxTopStartAnchor
			BhvrOverride
				PositionOffset 0 2.25 0
				PyrRotate 0 0 0
			End
		End

		Event
			At Root
			Type Start
			EName BoxBottomAnchor
			BhvrOverride
				PositionOffset 0 0 0
			End
		End

		Event
			At BoxBottomAnchor
			Type Local
			Geom _SS_PB_Pandora_Box_Bottom
		End

		Event
			At BoxBottomAnchor
			Type Local
			Bhvr behaviors\fadeout.bhvr
			Geom _SS_PB_Pandora_Box_decal_bottom
			LifeSpan 180
		End

		Event
			At BoxBottomAnchor
			Type Local
			BhvrOverride
				PositionOffset 0 2.25 0
				Scale 1 0.8 1
				StartColor 255 220 50
			End
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Start
			EName BoxTopBlowOutAnchor
			Bhvr :BoxTopBlowOut.bhvr
			BhvrOverride
				PositionOffset 0 2.25 0
				Gravity 0.04
				InitialVelocity 0.5 1.2 0
				InitialVelocityJitter 0.1 0 0.3
				PyrRotate 0 0 0
				PyrRotateJitter 90 0 0
				Scale 1 1 1
				#Spin 0 0 0
				#SpinJitter 1 1 1
				physDFriction 0.1
				physGravity 1
				physRestitution 0.3
				physSFriction 1
				physForcePower 100
				physForceRadius 0
				physForceType None
			End
			Geom _SS_PB_Pandora_Box_Top
			Cevent :Dust_Impact.fx
		End

		Event
			At BoxTopBlowOutAnchor
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 -1 0
			End
			Part Scripted\SignatureStories\PandorasBox\Box_PowerSplode_Streaks.part
			Part Scripted\SignatureStories\PandorasBox\Box_PowerSplode_Streaks2.part
			Part Scripted\SignatureStories\PandorasBox\Box_PowerSplode_Clouds_Lingering.part
			Part Scripted\SignatureStories\PandorasBox\Box_PowerSplode_Clouds.part
			Part Scripted\SignatureStories\PandorasBox\Box_PowerSplode_Shockwave.part
			Part Scripted\SignatureStories\PandorasBox\Box_PowerSplode_Shockwave_LocalFacing.part
			Part Scripted\SignatureStories\PandorasBox\Box_PowerSplode_Flash01.part
			Part Scripted\SignatureStories\PandorasBox\Box_PowerSplode_Flash02.part
			Part Scripted\SignatureStories\PandorasBox\Box_PowerSplode.part
			LifeSpan 12
		End

		Event
			At BoxTopBlowOutAnchor
			Type Local
			Bhvr behaviors\fadeout.bhvr
			Geom _SS_PB_Pandora_Box_decal_top
			LifeSpan 180
		End

	End

	Condition
		On Time
		Time 0

		Event
			At BoxTopStartAnchor
			Type Start
			Bhvr behaviors/CameraShakeHydra.bhvr
			BhvrOverride
				ShakeFallOff 0.005
			End
			Part :Nuke_Fire_Rising_Column.part
			Part :Nuke_Shockwave.part
			LifeSpan 12
		End

		Event
			At BoxTopStartAnchor
			Type Start
			Part :Nuke_Smoke_Cloud.part
			LifeSpan 30
		End

		Event
			At BoxTopStartAnchor
			Type Local
			Part :Nuke_Smoke_Ring_Central.part
			Part :Nuke_Shockwave_Flat.part
			LifeSpan 20
		End

		Event
			At BoxTopStartAnchor
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :FlareUp.part
			Part :UpSwooshes.part
			Part Scripted\SignatureStories\PandorasBox\BlackMatter_Rising.part
			LifeSpan 125
		End

#		Event
#			Type Start
#			CameraSpace
#			Bhvr Behaviors\GenericParticleFade.bhvr
#			Part :Camera_Dark.part
#			Part :Camera_Dark_Clouds.part
#			LifeSpan 10
#		End

	End

	Condition
		On Time
		Time 35

		Event
			At BoxTopStartAnchor
			Type StartPositOnly
			Part :Nuke_Smoke_Column.part
			Part :Nuke_Smoke_Cloud_Lingering.part
			LifeSpan 45
		End

#		Event
#			Type Start
#			CameraSpace
#			Bhvr Behaviors\GenericParticleFade.bhvr
#			Part :CAMERA_Debris.part
#			LifeSpan 40
#		End
#
#		Event
#			Type Start
#			CameraSpace
#			Bhvr Behaviors\GenericParticleFade.bhvr
#			Part :CAMERA_Dust.part
#			LifeSpan 80
#		End
#
#		Event
#			Type Start
#			CameraSpace
#			Bhvr Behaviors\GenericParticleFade.bhvr
#			Part :CAMERA_Dust.part
#			LifeSpan 60
#		End

	End

	Condition
		On Time
		Time 45

		Event
			At BoxTopStartAnchor
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 -15 0
			End
			Part :Nuke_Smoke_Column_Lingering.part
			LifeSpan 75
		End

	End
#
#	Condition
#		On Time
#		Time 55
#
#		Event
#			Type Start
#			CameraSpace
#			Bhvr Behaviors\GenericParticleFade.bhvr
#			Part :Camera_Dust_Still.part
#			LifeSpan 30
#		End
#
#	End

End
