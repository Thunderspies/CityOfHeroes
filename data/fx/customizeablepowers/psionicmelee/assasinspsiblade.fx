#
FxInfo

	LifeSpan 100


###########################################################

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
			Lifespan 36
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
			Lifespan 40
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
		Time 1

		Event
			Ename ShortBlade
			At BladeAnchor
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Blade_Base_Rings.part
			Part :Blade_Base_Glow.part
			Lifespan 48
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
			Lifespan 48
		End
	End

#	Condition
#		On Time
#		Time 18
#
#		Event
#			Ename BladeProjAnchor
#			At Root
#			Type Local
#			BhvrOverride
#				PositionOffset 0 3 0
#				PyrRotate -100 0 -5
#			End
#		End
#
#		Event
#			EName PRIME
#			At BladeProjAnchor
#			Type Start
#			LookAt Target
#			Magnet Target
#			Bhvr behaviors\Braz\Projectile_Fast_01.bhvr
#			BhvrOverride
#				Trackrate 2
#			End
#			Part :Sword_Projectile_Solid.part
#			Part :Sword_Projectile_Blur.part
#			Lifespan 9
#		End
#	End

#	Condition
#		On Time
#		Time 21
#
#		Event
#			EName PRIME2
#			At BladeProjAnchor
#			Type Start
#			LookAt Target
#			Magnet Target
#			Bhvr behaviors\Braz\Projectile_Fast_01.bhvr
#			BhvrOverride
#				Trackrate 2
#			End
#		#	Part :Sword_Projectile_Solid.part
#			Part :Sword_Projectile_Blur.part
#			Lifespan 9
#		End
#	End
#
#	Condition
#		On PRIMEHIT
#
#		Event
#			Ename Prime
#			Type Destroy
#		End
#	End
#
#	Condition
#		On PRIME2HIT
#
#		Event
#			Ename Prime2
#			Type Destroy
#		End
#	End
End
