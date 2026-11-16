#
FxInfo

	LifeSpan 100


###########################################################
// Condition
// 	On 	Time
// 	Time	0
// 	Event
// 		Type	Local
// 		At	Root
// 		Sound 	PsionicMelee_PsiBladeSweep 100 100 .8
// 	End
// End

	Condition
		On Time
		Time 1

		Event
			At Head
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.3 0
			End
			Part :PsioHeadRings01.part
			Part :PsioHeadRings02.part
			Part :PsioHeadGlow.part
			Part CustomizeablePowers\PsionicAssault\InfuseLight2.part
			Lifespan 20
		End

		Event
			At Head
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.3 0
			End
			Part :EnergyCoreFlash.part
			Part :InfuseLight2.part
			Lifespan 30
		End
	End

	Condition
		On Time
		Time 1

		Event
			Ename BladeAnchor
			At LarmR
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 	0.8 0.1 0.05
			End
		End

		Event
			Ename BladeAnchor2
			At LarmL
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 	-0.8 0.1 0.05
			End
		End
	End

	Condition
		On Time
		Time 12

		Event
			Ename ShortBlade
			At BladeAnchor
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Blade_Base_Rings.part
			Part :Blade_Base_Glow.part
			Lifespan 25
		End

		Event
			Ename ShortBlade
			At BladeAnchor
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0.2 0 0
				PyrRotate 0 90 0
			End
			Part :Blade_Core.part
			Part :Blade_Core_Chunks.part
			Part :Blade_Core2.part
			Part :Blade_Flame_Bits.part
			Part :Blade_Highlights.part
			Part :Blade_Trail.part
			Lifespan 25
		End

		Event
			Ename ShortBlade
			At BladeAnchor2
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Blade_Base_Rings.part
			Part :Blade_Base_Glow.part
			Lifespan 25
		End

		Event
			Ename ShortBlade
			At BladeAnchor2
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0.2 0 0
				PyrRotate 0 -90 0
			End
			Part :Blade_Core.part
			Part :Blade_Core_Chunks.part
			Part :Blade_Core2.part
			Part :Blade_Flame_Bits.part
			Part :Blade_Highlights.part
			Part :Blade_Trail.part
			Lifespan 25
		End
	End

	Condition
		On Time
		Time 28

		Event
			Ename Swoosh
			Type local
			At LarmR
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PyrRotate 0 -60 -40
			End
			Part :Swoosh_Trail.part
		End

		Event
			Ename Swoosh
			Type local
			At LarmL
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PyrRotate 180 -120 40
			End
			Part :Swoosh_Trail.part
		End
	End

	Condition
		On Time
		Time 25

		Event
			Ename BladeProjAnchor_A
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 3 0
				PyrRotate -100 0 -5
			End
		End

		Event
			EName PRIME
			At BladeProjAnchor_A
			Type Start
			LookAt Target
			Magnet Target
			Bhvr behaviors\Braz\Projectile_Fast_01.bhvr
			BhvrOverride
				Trackrate 2
			End
			Part :Sword_BigProjectile_Solid.part
			Part :Sword_BigProjectile_Blur.part
			Part :Sword_BigProjectile_Solid_Local.part
			Part :Sword_BigProjectile_Blur_Local.part
			Lifespan 9
		End
	End

	Condition
		On Time
		Time 28

		Event
			EName PRIME2
			At BladeProjAnchor_A
			Type Start
			LookAt Target
			Magnet Target
			Bhvr behaviors\Braz\Projectile_Fast_01.bhvr
			BhvrOverride
				Trackrate 2
			End
			Part :Sword_BigProjectile_Solid.part
			Part :Sword_BigProjectile_Blur.part
			Part :Sword_BigProjectile_Solid_Local.part
			Part :Sword_BigProjectile_Blur_Local.part
			Lifespan 9
		End
	End

	Condition
		On PRIMEHIT

		Event
			Ename Prime
			Type Destroy
		End
	End

	Condition
		On PRIME2HIT

		Event
			Ename Prime2
			Type Destroy
		End
	End
End
