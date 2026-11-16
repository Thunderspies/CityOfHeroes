#
FxInfo

	Flags DontSuppress

	LifeSpan 500


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			Sound Magisterium_NuclearExplosion 100 100 0.8
		End

	End

	Condition
		On Time
		Time 0

		Event
			Type Local
			#ChildFx :Nuke_ScreenFX.fx
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Start
			EName Anchor
		End

		Event
			Type	StartPositOnly
			At	Root
			Bhvr	behaviors/CameraShake02.bhvr
		End

		Event
			At Anchor
			Type Local
			EName MainAnchor
			BhvrOverride
				PositionOffset 0 0 0
			End
			Part :Nuke_Glow_Rising.part
			Part :Nuke_Flash.part
		End

	End

	Condition
		On Time
		Time 3

		Event
			At MainAnchor
			Type Local
			BhvrOverride
				PositionOffset 0 0 0
			End
			Part :Flash_Glow_Large.part
			Part :Flash_Sparks.part
			Part :Sparks_Impact.part
		End

		Event
			At MainAnchor
			Type Local
			BhvrOverride
				PositionOffset 65 50 -2
			End
			Part :Nuke_Fire_Initial.part
		End

		Event
			At MainAnchor
			Type Local
			Part :Dirt_Splatter.part
			Part :Dust_Pillar_Large.part
		End

	End

	Condition
		On Time
		Time 7

		Event
			At MainAnchor
			Type Local
			BhvrOverride
				PositionOffset -65 50 -2
			End
			Part :Nuke_Fire_Initial.part
		End

	End

	Condition
		On Time
		Time 10

		Event
			At MainAnchor
			Type Local
			EName MushroomAnchor
			BhvrOverride
				InitialVelocity 0 1 0
				PositionOffset 0 50 0
			End
		End

		Event
			At MushroomAnchor
			Type Local
			LookAt MainAnchor
			POther MainAnchor
			Bhvr Events\EndGame\Magisterium\NukeHead.bhvr
			BhvrOverride
				Gravity -0.015
				PositionOffset 0 100 0
			End
			Part :Nuke_Smoke_Head_Continuing.part
			Part :Nuke_Smoke_Head_Continuing2.part
			Part Events\EndGame\Magisterium\Nuke_Smoke_Head_Continuing3.part
			Part Events\EndGame\Magisterium\Nuke_Smoke_Head_Big.part
			Part :Nuke_Fire_Head_Big.part
			Part :Nuke_Fire_Head_Continuing.part
			Part :Nuke_Fire_Head_Continuing2.part
			#Part Events\EndGame\Magisterium\Nuke_Fire_Head_FireBellow.part
			Part Events\EndGame\Magisterium\Nuke_Smoke_Column.part
			Part Events\EndGame\Magisterium\Nuke_Smoke_Column.part
			LifeSpan 230
		End

		Event
			At MainAnchor
			Type Local
			BhvrOverride
				PositionOffset 80 50 0
			End
			Part :Nuke_Fire_Initial.part
		End

	End

	Condition
		On Time
		Time 15

		Event
			At MainAnchor
			Type Local
			BhvrOverride
				PositionOffset -80 50 0
			End
			Part :Nuke_Fire_Initial.part
		End

		Event
			At MainAnchor
			Type Local
			Bhvr Events\EndGame\Magisterium\Ground_Dust_Expand.bhvr
			BhvrOverride
				PositionOffset 0 0 0
				EndScale 6 6 6
			End
			Part :Nuke_Smoke_Ring_Initial.part
		End

		Event
			At MainAnchor
			Type Local
			BhvrOverride
				PositionOffset 0 100 0
			End
			Part :Nuke_Smoke_Initial.part
		End

		Event
			At MainAnchor
			Type Local
			BhvrOverride
				PositionOffset 0 60 0
			End
			Part :Nuke_Smoke_Initial_Dark.part
		End

	End

	Condition
		On Time
		Time 7

		Event
			At MushroomAnchor
			Type Local
			Bhvr behaviors/CameraShake02.bhvr
			BhvrOverride
				PositionOffset 0 -7.5 0
				ShakeFallOff 0.005
				ShakeRadius 2000
			End
			Part :Nuke_Shockwave.part
			LifeSpan 10
		End

		Event
			At MainAnchor
			Type Local
			EName RingAnchor
			Bhvr Events\EndGame\Magisterium\Ground_Dust_Expand.bhvr
			Part :Nuke_Shockwave_Flat.part
			Part :Nuke_Shockwave_Flat_Dark.part
			LifeSpan 10
		End

		Event
			At MainAnchor
			Type Local
			EName RingAnchor
			Bhvr Events\EndGame\Magisterium\Ground_Dust_Expand2.bhvr
			BhvrOverride
				PositionOffset 0 20 0
			End
			Part :Nuke_Smoke_Ring_Expanding.part
			#LifeSpan 10
		End

	End

	Condition
		On Time
		Time 30

		Event
			Type	StartPositOnly
			At	Root
			Bhvr	behaviors/CameraShake_Subtle90.bhvr
			BhvrOverride
				Shake          0.15
				ShakeFallOff   .001
			End
		End

	End

	Condition
		On Time
		Time 90

		Event
			Type	StartPositOnly
			At	Root
			Bhvr	behaviors/CameraShake_Subtle90.bhvr
			BhvrOverride
				Shake          0.5
#				ShakeFallOff   .001
			End
		End

	End

	Condition
		On Time
		Time 160

		Event
			At MainAnchor
			Type Local
			EName RingAnchor
			Bhvr Events\EndGame\Magisterium\Upper_Dust_Expand.bhvr
			BhvrOverride
				PositionOffset 0 800 0
				PyrRotate 0 -43 0
			End
			Part Events\EndGame\Magisterium\Nuke_Upper_Shockwave_Flat.part
			#Part :Nuke_Shockwave_Flat.part
		End

	End

End
