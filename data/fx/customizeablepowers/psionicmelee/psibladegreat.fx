FXinfo
Lifespan 150

// Condition
// 	On 	Time
// 	Time	0
// 	Event
// 		Type	Local
// 		At	Root
// 		Sound 	PsionicMelee_PsiBladeGreat 100 100 .8
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
			Lifespan 60
		End

		Event
			Ename BladeAnchor
			At LarmR
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 	0.8 0.1 0.05
			End
		End
	End

	Condition
		On Time
		Time 8

		Event
			Ename ShortBlade
			At BladeAnchor
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Blade_Base_Rings.part
			Part :Blade_Base_Glow.part
			Lifespan 15
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
			Lifespan 15
		End
	End

	Condition
		On Time
		Time 23

		Event
			Ename BigBladeAnchor
			At BladeAnchor
			Type Local
			BhvrOverride
				Positionoffset 0 0 4
			End
		End

		Event
			Ename ShortBlade
			At BladeAnchor
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :GreatBlade_Base_SolidRings.part
			Part :GreatBlade_Base_Rings.part
			Part :GreatBlade_Base_Glow.part
			Lifespan 45
		End

		Event
			Ename ShortBlade
			At BladeAnchor
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0.2 0 0
				PyrRotate 0 70 0
			End
			Part :GreatBlade_CoreFill.part
			Part :GreatBlade_Core.part
			Part :GreatBlade_Core_Chunks.part
			Part :GreatBlade_Core2.part
			Part :GreatBlade_Flame_Bits.part
			Part :GreatBlade_Highlights.part
			Lifespan 45
		End
	End

	Condition
		On time
		Time 47

		Event
			Ename Swoosh
			Type local
			At LarmR
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PyrRotate 0 -70 -0
				PositionOffset 1 0 1
			End
			Part :Swoosh_BigTrail.part
		End

	End

	Condition
		On Time
		Time 65

		Event
			Ename ShortBlade
			At BladeAnchor
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Blade_Base_Rings.part
			Part :Blade_Base_Glow.part
			Lifespan 20
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
			Lifespan 20
		End
	End

	Condition
		On Time
		Time 45

		Event
			Ename BladeProjAnchor_A
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 6 0
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
			Lifespan 9
		End
	End

	Condition
		On Time
		Time 50
#
#		Event
#			Ename	TopAnchor
#			At	Root
#			Type	Local
#			BhvrOverride
#				TintGeom 	1
#				PositionOffset 	0 4 4
#				PyrRotate	-10 180 10
#				Scale		1.25 1.25 1.25
#			End
#			Geom FX_Slash
#			Lifespan 4
#		End

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
		#	Part :Sword_BigProjectile_Solid.part
			Part :Sword_BigProjectile_Blur.part
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
