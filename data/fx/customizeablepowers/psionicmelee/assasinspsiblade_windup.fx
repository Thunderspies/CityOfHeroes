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
		Time 15

		Event
			Ename ShortBlade
			At BladeAnchor
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :AssBlade_Charge_SolidRings.part
			Part :AssBlade_Charge_Rings.part
			#Part :Blade_Base_Rings.part
			#Part :Blade_Base_Glow.part
			Part :PsioHeadGlow.part
			#Part :AssBlade_Charge_Glow.part
			Part CustomizeablePowers\Brawling\Hand_Buildup_Ring_Continuing_Smokey.part
			Part CustomizeablePowers\Brawling\Hand_Buildup_Ring_Continuing.part
			#Lifespan 15
		End
	End

End
