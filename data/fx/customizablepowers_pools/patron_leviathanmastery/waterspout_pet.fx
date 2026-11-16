#                                                          
FxInfo


###########################################################

	Condition
		On Time
		Time 20

		Event
			At Root
			Type Local
			EName Ring1
			Bhvr behaviors/SuperSpeedWirlwind.bhvr
			Part :Steam.part
			Part :WaterDrops.part
			Part :SteamBright.part
			Part :WaterDroplets.part
			Part :Rocks.part
			Part :Sand.part
			Part :SandBright.part
			Part :SandFlat.part
			Part :SandBrightFlat.part
			Part :SandBrightCore.part
			Part :WaterDropsCore.part
			Part :WaterDropsIn.part
			Sound Whirlwind2_loop 80 80 0.5
		End

		Event
			At Root
			Type Local
			EName SwirlForce1
			BhvrOverride
				PositionOffset 0 6 0
				physForceType CCWSwirl
				physForceRadius 10
				physForceCentripetal .65
				physForcePower 100
				physForcePowerJitter 5
			End
			Sound WaterFountain2_loop 80 80 1
			#LifeSpan 40
		End

		Event
			At Root
			Type Local
			EName SwirlForce1
			BhvrOverride
				PositionOffset 0 16 0
				physForceType CCWSwirl
				physForceRadius 12
				physForceCentripetal 1
				physForcePower 100
				physForcePowerJitter 5
			End
			#LifeSpan 40
		End

		Event
			At Root
			Type Local
			EName LevitationForce1
			BhvrOverride
				PositionOffset 0 5 0
				physForceType Up
				physForceRadius 12
				physForcePower 50
				physForcePowerJitter 5
			End
			#LifeSpan 30
		End

		Event
			At Root
			Type Local
			EName LevitationForce2
			BhvrOverride
				PositionOffset 0 15 0
				physForceType Up
				physForceRadius 10
				physForcePower 25
				physForcePowerJitter 5
			End
			#LifeSpan 30
		End

		Event
			At Root
			Type Local
			EName SpreadForce
			BhvrOverride
				PositionOffset 0 0 0
				physForceType Out
				physForceRadius 15
				physForcePower 25
				physForcePowerJitter 5
			End
			#LifeSpan 30
		End

		Event
			At Root
			Type Local
			EName Shhot
			BhvrOverride
				PositionOffset 0 16 0
				physForceType Out
				physForceRadius 5
				physForcePower 200
				physForcePowerJitter 5
			End
			#LifeSpan 30
		End

	End

	Condition
		On Cycle
		Time 12

		Event
			At Root
			Type Local
			EName Rings
			Bhvr behaviors/WaterSpoutMotion.bhvr
			BhvrOverride
				TintGeom 1
				Alpha 15
			End
			Geom Tintable_FX_TmpCtrlWisp01
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 24

		Event
			At Root
			Type Local
			Bhvr behaviors/WaterSpoutCore.bhvr
			BhvrOverride
				TintGeom 1
			End
			Geom Tintable_WaterFunnel
		End

	End

End
