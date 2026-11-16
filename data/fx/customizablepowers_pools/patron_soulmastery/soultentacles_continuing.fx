#
FxInfo

###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			Flags PowerLoopingSound
			Bhvr behaviors/soundFade3.bhvr
			Sound tentacle_loop 80 80 0.6
			LifeSpan 420
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			EName Base
			Part :TendrilPuddle.part
			Part :TendrilPuddle2.part
			Part :SoulTendrilRing.Part
			Part :SoulTendrilsEmber.part
			Sound grexplo2 80 80 0.6
		End

		Event
			At Root
			Type Start
			EName Explode
			Part :soulFlash1.part
			Part :soulFlash2.part
			Part :soulExplode1.part
			Part :soulExplode2.part
			Part :soulSparks2.part
			LifeSpan 5
		End

		Event
			At LLegR
			Type Local
			EName glow
			Bhvr behaviors/genericParticleFade.bhvr
			Part :soulChoke1.part
		End

		Event
			At LLegL
			Type Local
			EName glow
			Bhvr behaviors/genericParticleFade.bhvr
			Part :soulChoke1.part
		End

		Event
			At FootR
			Type Local
			EName glow
			Bhvr behaviors/genericParticleFade.bhvr
			Part :soulChoke1.part
			Part :soulSuck1.part
		End

		Event
			At FootL
			Type Local
			EName glow
			Bhvr behaviors/genericParticleFade.bhvr
			Part :soulChoke1.part
			Part :soulSuck1.part
		End

	End

	Condition
		On Time
		Time 4

		Event
			Anim FX_tenticlebase
			At Root
			Type Local
			EName Prime
			Bhvr behaviors/ScaleTenticle.bhvr
		End

		Event
			Anim FX_GhostWidow_Tentacle_B_Tintable
			AnimPiv Chest
			At Prime
			Type Local
			EName tenticle_1
			Bhvr behaviors/TentacleFade.bhvr
			BhvrOverride
				TintGeom 1
			End
			Part :soulSmokeRing.part
		End

		Event
			Anim FX_GhostWidow_Tentacle_Tintable
			AnimPiv Neck
			At Prime
			Type Local
			EName tenticle_2
			Bhvr behaviors/TentacleFade.bhvr
			BhvrOverride
				TintGeom 1
			End
			Part :soulSmokeRing.part
		End

		Event
			Anim FX_GhostWidow_Tentacle_A_Tintable
			AnimPiv Head
			At Prime
			Type Local
			EName tenticle_3
			Bhvr behaviors/TentacleFade.bhvr
			BhvrOverride
				TintGeom 1
			End
			Part :soulSmokeRing.part
		End

		Event
			Anim FX_GhostWidow_Tentacle_Tintable
			AnimPiv Hair
			At Prime
			Type Local
			EName tenticle_4
			Bhvr behaviors/TentacleFade.bhvr
			BhvrOverride
				TintGeom 1
			End
			Part :soulSmokeRing.part
		End

		Event
			Anim FX_GhostWidow_Tentacle_A_Tintable
			AnimPiv Waist
			At Prime
			Type Local
			EName tenticle_5
			Bhvr behaviors/TentacleFade.bhvr
			BhvrOverride
				TintGeom 1
			End
			Part :soulSmokeRing.part
		End

	End

	Condition
		On Time
		Time 10

		Event
			Anim FX_tenticlebase
			At Root
			Type Local
			EName Prime2
			Bhvr behaviors/ScaleTenticleOutside.bhvr
		End

		Event
			Anim FX_GhostWidow_Tentacle_B_Tintable
			AnimPiv Chest
			At Prime2
			Type Local
			EName tenticle_11
			Bhvr behaviors/TentacleFade.bhvr
			BhvrOverride
				TintGeom 1
			End
			Part :soulSmokeRing.part
		End

		Event
			Anim FX_GhostWidow_Tentacle_Tintable
			AnimPiv Neck
			At Prime2
			Type Local
			EName tenticle_21
			Bhvr behaviors/TentacleFade.bhvr
			BhvrOverride
				TintGeom 1
			End
			Part :soulSmokeRing.part
		End

		Event
			Anim FX_GhostWidow_Tentacle_A_Tintable
			AnimPiv Head
			At Prime2
			Type Local
			EName tenticle_31
			Bhvr behaviors/TentacleFade.bhvr
			BhvrOverride
				TintGeom 1
			End
			Part :soulSmokeRing.part
		End

		Event
			Anim FX_GhostWidow_Tentacle_Tintable
			AnimPiv Hair
			At Prime2
			Type Local
			EName tenticle_41
			Bhvr behaviors/TentacleFade.bhvr
			BhvrOverride
				TintGeom 1
			End
			Part :soulSmokeRing.part
		End

		Event
			Anim FX_GhostWidow_Tentacle_A_Tintable
			AnimPiv Waist
			At Prime2
			Type Local
			EName tenticle_51
			Bhvr behaviors/TentacleFade.bhvr
			BhvrOverride
				TintGeom 1
			End
			Part :soulSmokeRing.part
		End

	End

End
